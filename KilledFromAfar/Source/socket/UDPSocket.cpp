#include "socket\UDPSocket.h"
#include "NetworkManager.h"
#include "data\messages\NetcodeMessages.h"

using namespace Net13;

UDPSocket::UDPSocket()
	// Connection
	: m_Connecting(false)
	, m_Connected(false)
	, m_ConnectionID(0)
	, m_ConnectionIDGenerator(UINT16_MAX)
	, m_ConnectClock(0)
	, m_ConnectionAttempts(0)
	
	// Tick rates
	, m_TickRateSend(0)
	, m_ClockPerTickSend(0)
	, m_SendClock(clock())

	// Socket configuration
	, m_Port(0)

	, m_pPacketHandler(nullptr)
{
	m_ConnectionIDGenerator.Skip(2); // Make sure clients start with ID 2, so that 0 can be used for invalid IDs and 1 (kDefaultServerID) for the server

	Messages::ACK ack;
	m_ACKMessageSize = ack.GetMessageSize();
}

UDPSocket::~UDPSocket()
{
}

void UDPSocket::ConnectTo(const char* acpIP, uint16_t aPort)
{
	m_Connections.clear();

	IPAddress ip;
	ip.m_Address = acpIP;
	ip.m_Port = aPort;

	// When connecting to a server, the server is always connection id 1 (kServerConnectionID), as 0 is for uninitialized/invalid connection IDs
	// Also don't allow the client to send heartbeats - the server should be managing that
	m_Connections.push_back(Connection(ip, kServerConnectionID, false));

	m_Connected = false;
	m_Connecting = true;

	m_ConnectionAttempts = 0;
	m_ConnectClock = clock() - kRetryConnectInterval; // Start right away

	g_Network->GetDebugger()->Print("Connecting to " + ip.m_Address + ":" + std::to_string(aPort) + "...");
}

void UDPSocket::Disconnect()
{
	if (m_Connected)
	{
		//g_Network->GetDebugger()->Print("Saying goodbye to everyone\n");
		// Send disconnect message to everyone
		auto pDisconnectMessage = std::make_shared<Messages::Disconnect>();
		pDisconnectMessage->connectionID = m_ConnectionID;
		pDisconnectMessage->reason = Messages::EDisconnectReason::REASON_QUIT;

		// Send callback to application
		g_Network->GetNetworkSystem()->OnDisconnect(*pDisconnectMessage);

		// Send message
		g_Network->Send(pDisconnectMessage, kServerConnectionID);
		SendPackets(true);

		m_Connected = false;
	}
}

void UDPSocket::Update()
{
	Connect();
	ReceivePackets();
	UpdateConnections();
	SendPackets(true);
	UpdateStatistics();
}

void UDPSocket::AddMessage(std::shared_ptr<Messages::Message> apMessage, uint16_t aConnectionID, bool aStoreACK)
{
	// Add our connectionID to the message
	apMessage->m_ConnectionID = m_ConnectionID;

	if (aConnectionID == 0)
	{
		for (auto& connection : m_Connections)
		{
			if (!connection.IsActive()) continue;
			connection.AddMessage(apMessage, aStoreACK, (aStoreACK ? m_ACKMessageSize : 0));
		}
	}
	else
	{
		Connection* pConnection = GetConnection(aConnectionID);

		if (pConnection)
		{
			pConnection->AddMessage(apMessage, aStoreACK, (aStoreACK ? m_ACKMessageSize : 0));
		}
	}
}

void UDPSocket::SetTickRate(uint8_t aTickRateSend)
{
	m_TickRateSend = aTickRateSend;
	m_ClockPerTickSend = clock_t(1000.f / (float)aTickRateSend);
}

void UDPSocket::ProcessHeartbeat(const Messages::Heartbeat& acMessage)
{
	for (auto& connection : m_Connections)
	{
		if (connection.GetConnectionID() == acMessage.connectionID)
		{
			connection.ProcessHeartbeat(acMessage);
			return;
		}
	}
}

uint16_t UDPSocket::ProcessIPAddress(const IPAddress& acIPAddress)
{
	if (g_Network->IsServer())
	{
		// Checks if the connection is new. If so, create a new Connection
		// Returns the ConnectionID of the connection
		for (auto& connection : m_Connections)
		{
			if (connection.GetIPAddress() == acIPAddress && connection.IsActive())
			{
				return connection.GetConnectionID();
			}
		}

		// No connection found - create new one
		uint16_t connectionID = m_ConnectionIDGenerator.GenerateID();
		g_Network->GetDebugger()->Print("Adding " + acIPAddress.m_Address  + ":" + std::to_string(acIPAddress.m_Port) + " as a new connection with connection ID " + std::to_string(connectionID) + "...");

		m_Connections.push_back(Connection(acIPAddress, connectionID, g_Network->GetNetworkSystem()->IsServer()));
		return connectionID;
	}
	else
	{
		return kServerConnectionID;
	}
}

void UDPSocket::ProcessACKAcknowledgment(const Messages::AcknowledgeACK& acACK)
{
	Connection* pConnection = GetConnection(acACK.connectionID);

	if (pConnection)
	{
		pConnection->RemoveCachedACKMessage(acACK.id);
	}
	else
	{
		g_Network->GetDebugger()->Print("ERROR: Trying to Acknowledge ACK for a connection that doesn't exist! Connection ID: " + std::to_string(acACK.connectionID) + "\n");
	}
}

void UDPSocket::SetConnected(bool aVal)
{
	m_Connected = aVal;
	m_Connecting = false;

	if (!m_Connected)
	{
		m_Connections.clear();
	}
}

const bool UDPSocket::IsConnected() const
{
	if (g_Network->IsServer())
	{
		return true;
	}
	else
	{
		return m_Connected;
	}
}

void UDPSocket::AddConnection(const IPAddress& aIPAddress, bool aSendHeartbeats)
{
	m_Connections.push_back(Connection(aIPAddress, int16_t(m_Connections.size()), false));
}

Connection* UDPSocket::GetNewestConnection()
{
	if (m_Connections.size() > 0)
	{
		return &m_Connections.back();
	}
	else
	{
		return nullptr;
	}
}

Connection* UDPSocket::GetConnection(uint16_t aID)
{
	if (aID == 0) return nullptr;

	for (auto& connection : m_Connections)
	{
		if (connection.GetConnectionID() == aID)
		{
			return &connection;
		}
	}

	return nullptr;
}

const uint8_t UDPSocket::GetActiveConnectionsCount() const
{
	uint8_t count = 0;

	for (auto& connection : m_Connections)
	{
		if (connection.IsActive())
		{
			count++;
		}
	}

	return count;
}

void UDPSocket::SetConnectionID(uint16_t aVal)
{
	m_ConnectionID = aVal;
}

const uint16_t UDPSocket::GetConnectionID() const
{
	return m_ConnectionID;
}

const uint16_t UDPSocket::GetPort() const
{
	return m_Port;
}

const uint32_t UDPSocket::GetIncomingPacketsPerSecond() const
{
	return uint32_t(m_PPSIn.size());
}

const uint32_t UDPSocket::GetOutgoingPacketsPerSecond() const
{
	return uint32_t(m_PPSOut.size());
}

const uint32_t UDPSocket::GetPacketsPerSecondLost() const
{
	return uint32_t(m_PPSLost.size());
}

const uint32_t UDPSocket::GetIncomingDataPerSecond() const
{
	if (m_DataPerSecondReceived.size() == 0) return 0;

	uint32_t dataSize = 0;

	for (auto it : m_DataPerSecondReceived)
	{
		dataSize += it.dataSize;
	}

	return dataSize;
}

const uint32_t UDPSocket::GetOutgoingDataPerSecond() const
{
	if (m_DataPerSecondSent.size() == 0) return 0;

	uint32_t dataSize = 0;

	for (auto it : m_DataPerSecondSent)
	{
		dataSize += it.dataSize;
	}

	return dataSize;
}

void UDPSocket::Connect()
{
	if (!m_Connecting || m_Connected) return;

	if (clock() - m_ConnectClock < kRetryConnectInterval) return;
	m_ConnectClock = clock();

	auto pRequest = std::make_shared<Messages::ConnectionRequest>();
	g_Network->Send(pRequest, kServerConnectionID);

	m_ConnectionAttempts++;

	if (m_ConnectionAttempts >= kConnectionAttempts)
	{
		g_Network->GetDebugger()->Print("Failed to connect to the server: No response.");
		m_Connected = false;
		m_Connecting = false;
	}
	else
	{
		g_Network->GetDebugger()->Print("No response yet, retrying...");
	}
}

void UDPSocket::SendPackets(bool aForceSend)
{
	if (!m_Connected && !m_Connecting) return;

	// Tick rate
	if (!aForceSend)
	{
		if (clock() - m_SendClock < m_ClockPerTickSend) return;
		m_SendClock = clock();
	}

	// Send packets
	Send();
}

void UDPSocket::ReceivePackets()
{
	// Tick rate
	if (!m_Connected && !m_Connecting) return;

	// Receive data
	Receive();
}

void UDPSocket::UpdateConnections()
{
	for (auto& connection : m_Connections)
	{
		connection.Update();
	}
}

void UDPSocket::UpdateStatistics()
{
	RemoveTimeIfExceedsAmount(&m_PPSOut);
	RemoveTimeIfExceedsAmount(&m_PPSIn);
	RemoveTimeIfExceedsAmount(&m_PPSLost);
	RemoveTimeIfExceedsAmount(&m_DataPerSecondSent);
	RemoveTimeIfExceedsAmount(&m_DataPerSecondReceived);
}

void UDPSocket::RemoveTimeIfExceedsAmount(std::list<clock_t>* apList, float aTime)
{
	if (apList->size() == 0) return;

	const clock_t now = clock();

	auto it = apList->begin();
	while (it != apList->end())
	{
		auto prev_it = it;
		it++;
		if (now - *prev_it >= aTime)
		{
			apList->erase(prev_it);
		}
	}
}

void UDPSocket::RemoveTimeIfExceedsAmount(std::list<DataDebugInfo>* apList, float aTime)
{
	if (apList->size() == 0) return;

	const clock_t now = clock();

	auto it = apList->begin();
	while (it != apList->end())
	{
		auto prev_it = it;
		it++;
		if (now - (*prev_it).time >= aTime)
		{
			apList->erase(prev_it);
		}
	}
}

void UDPSocket::AddConnection(const IPAddress& aIPAddress, uint16_t aConnectionID)
{
	m_Connections.push_back(std::move(Connection(aIPAddress, aConnectionID, g_Network->GetNetworkSystem()->IsServer())));
}

// Handles
void Net13::Messages::AcknowledgeACK_Receive(const Messages::AcknowledgeACK& acMessage)
{
	g_Network->GetUDPSocket()->ProcessACKAcknowledgment(acMessage);
}

void Net13::Messages::Heartbeat_Receive(const Messages::Heartbeat& acMessage)
{
	g_Network->GetUDPSocket()->ProcessHeartbeat(acMessage);
}
