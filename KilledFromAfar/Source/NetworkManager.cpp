#include "NetworkManager.h"
#include "socket\windows\WindowsUDPSocket.h"
#include "systems\client\NetworkClient.h"
#include "systems\server\NetworkServer.h"

using namespace Net13;

Net13::NetworkManager* Net13::g_Network = nullptr;

NetworkManager::NetworkManager(uint16_t aPort, bool aIsServer)
	: m_isServer(aIsServer)
{
	g_Network = this;

	CreateSocket(aPort);

	if (aIsServer)
	{
		m_pNetworkSystem = std::make_unique<NetworkServer>();
		m_pUDPSocket->SetConnected(true);
		m_pUDPSocket->SetConnectionID(UDPSocket::kServerConnectionID);
	}
	else
	{
		m_pNetworkSystem = std::make_unique<NetworkClient>();
	}
}

NetworkManager::~NetworkManager()
{
	if (IsConnected())
	{
		Disconnect();
	}
}

void NetworkManager::CreateSocket(uint16_t aPort)
{
#if defined(_WIN64) || defined(_WIN32)
	m_pUDPSocket = std::make_unique<WindowsUDPSocket>(&m_PacketHandler, aPort, kDefaultTickRate);
#else
	// Linux?
#endif
}

void NetworkManager::Update()
{
#if _DEBUG
	m_NetworkSimulator.Update();
#endif
	m_pUDPSocket->Update();
}

void NetworkManager::ConnectTo(const char* apIP, uint16_t aPort)
{
	m_pUDPSocket->ConnectTo(apIP, aPort);
}

void NetworkManager::Disconnect()
{
	m_pUDPSocket->Disconnect();
}

void NetworkManager::Send(std::shared_ptr<Messages::Message> apMessage, uint16_t aConnectionID)
{
	apMessage->m_MessageType = Messages::EMessageType::MESSAGETYPE_UNRELIABLE;

	if (m_NetworkSimulator.IsSimulating())
	{
		m_NetworkSimulator.ProcessMessage(apMessage, aConnectionID, false);
	}
	else
	{
		m_pUDPSocket->AddMessage(apMessage, aConnectionID, false);
	}
}

void NetworkManager::SendReliable(std::shared_ptr<Messages::Message> apMessage, uint16_t aConnectionID, bool aStoreACK)
{
	apMessage->m_MessageType = Messages::EMessageType::MESSAGETYPE_RELIABLE;

	if (m_NetworkSimulator.IsSimulating())
	{
		m_NetworkSimulator.ProcessMessage(apMessage, aConnectionID, aStoreACK);
	}
	else
	{
		m_pUDPSocket->AddMessage(apMessage, aConnectionID, aStoreACK);
	}
}

void NetworkManager::SendSequenced(std::shared_ptr<Messages::Message> apMessage, uint16_t aConnectionID, bool aStoreACK)
{
	apMessage->m_MessageType = Messages::EMessageType::MESSAGETYPE_SEQUENCED;
	
	if (m_NetworkSimulator.IsSimulating())
	{
		m_NetworkSimulator.ProcessMessage(apMessage, aConnectionID, aStoreACK);
	}
	else
	{
		m_pUDPSocket->AddMessage(apMessage, aConnectionID, aStoreACK);
	}
}

void NetworkManager::RegisterMessage(std::unique_ptr<Messages::Message> apMessage, uint8_t aOpCode)
{
	m_PacketHandler.RegisterMessage(std::move(apMessage), aOpCode);
}

void NetworkManager::SetTickRate(uint8_t aSendTickRate)
{
	m_pUDPSocket->SetTickRate(aSendTickRate);
}

void NetworkManager::StartSimulation(const uint32_t acMinimumDelayMS, const uint32_t acRandomDelayMS, const float acPacketLossPct, const float acPacketDuplicationPct)
{
	m_NetworkSimulator.StartSimulation(acMinimumDelayMS, acRandomDelayMS, acPacketLossPct, acPacketDuplicationPct);
}

void NetworkManager::StopSimulation()
{
	m_NetworkSimulator.StopSimulation();
}

void NetworkManager::RegisterOnConnectionResultCallback(std::function<void(Net13::Messages::EConnectionResult)> aCallback)
{
	m_pNetworkSystem->RegisterOnConnectionResultCallback(aCallback);
}

void NetworkManager::RegisterOnDisconnectCallback(std::function<void(Net13::Messages::EDisconnectReason)> aCallback)
{
	m_pNetworkSystem->RegisterOnDisconnectCallback(aCallback);
}

void NetworkManager::RegisterOnPlayerAddCallback(std::function<void(uint16_t)> aCallback)
{
	m_pNetworkSystem->RegisterOnPlayerAddCallback(aCallback);
}

void NetworkManager::RegisterOnPlayerDisconnectCallback(std::function<void(uint16_t, Net13::Messages::EDisconnectReason)> aCallback)
{
	m_pNetworkSystem->RegisterOnPlayerDisconnectCallback(aCallback);
}

const bool NetworkManager::IsConnected() const
{
	return m_pUDPSocket->IsConnected();
}

const bool Net13::NetworkManager::IsServer() const
{
	return m_isServer;
}

NetworkSystem* Net13::NetworkManager::GetNetworkSystem()
{
	return m_pNetworkSystem.get();
}

UDPSocket* NetworkManager::GetUDPSocket()
{
	return m_pUDPSocket.get();
}

NetworkDebugger* Net13::NetworkManager::GetDebugger()
{
	return &m_NetworkDebugger;
}

const uint32_t Net13::NetworkManager::GetNewestPing(uint16_t aConnectionID) const
{
	if (m_pUDPSocket->IsConnected() == false) return 0;
	Connection* pConnection = m_pUDPSocket->GetConnection(aConnectionID);

	if (pConnection)
	{
		return pConnection->GetNewestPing();
	}
	else
	{
		return 0;
	}
}

const uint32_t Net13::NetworkManager::GetAveragePing(uint16_t aConnectionID) const
{
	if (m_pUDPSocket->IsConnected() == false) return 0;
	Connection* pConnection = m_pUDPSocket->GetConnection(aConnectionID);

	if (pConnection)
	{
		return pConnection->GetAveragePing();
	}
	else
	{
		return 0;
	}
}

const uint32_t NetworkManager::GetIncomingPacketsPerSecond() const
{
	return m_pUDPSocket->GetIncomingPacketsPerSecond();
}

const uint32_t NetworkManager::GetOutgoingPacketsPerSecond() const
{
	return m_pUDPSocket->GetOutgoingPacketsPerSecond();
}

const uint32_t NetworkManager::GetPacketsPerSecondLost() const
{
	return m_pUDPSocket->GetPacketsPerSecondLost();
}

const uint32_t NetworkManager::GetIncomingDataPerSecond() const
{
	return m_pUDPSocket->GetIncomingDataPerSecond();
}

const uint32_t NetworkManager::GetOutgoingDataPerSecond() const
{
	return m_pUDPSocket->GetOutgoingDataPerSecond();
}

const uint16_t NetworkManager::GetActiveConnectionsCount() const
{
	return m_pUDPSocket->GetActiveConnectionsCount();
}
