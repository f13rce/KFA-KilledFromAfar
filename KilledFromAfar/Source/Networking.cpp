#include "Networking.h"

Networking::Networking()
{
	m_receiveBuffer.resize(s_cMaxBufferSize);
}

Networking::~Networking()
{
}

void Networking::Update()
{
	Receive();
	Send();
}

void Networking::Send()
{
}

void Networking::Receive()
{
	int32_t receivedBytes = m_pSocket->Receive(&m_receiveBuffer[0], s_cMaxBufferSize);
	int32_t readIt = 0;

	while (readIt < receivedBytes)
	{
		NetworkMessageHeader header;

		if ((receivedBytes - readIt) < sizeof(NetworkMessageHeader))
		{
			std::cout << "Warning: Received data is incomplete!" << std::endl;
			break;
		}
		
		memcpy(&header, &m_receiveBuffer[readIt], sizeof(NetworkMessageHeader));
		readIt += sizeof(NetworkMessageHeader);

		bool success = false;
		switch (header.opcode)
		{
			case ENetworkMessages::ACK:
			{
				ACK msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnACK(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::ConnectionRequest:
			{
				ConnectionRequest msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnConnectionRequest(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::ConnectionResponse:
			{
				ConnectionResponse msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnConnectionResponse(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::Play2DSound:
			{
				Play2DSound msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnPlay2DSound(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::Play3DSound:
			{
				Play3DSound msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnPlay3DSound(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::PlayerInput:
			{
				PlayerInput msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg, &m_receiveBuffer[readIt], sizeof(msg));
					readIt += sizeof(msg);

					OnPlayerInput(msg);

					success = true;
				}

				break;
			}

			case ENetworkMessages::Frame:
			{
				Frame msg;
				if ((receivedBytes - readIt) < sizeof(msg))
				{
					break;
				}
				else
				{
					memcpy(&msg.sequence, &m_receiveBuffer[readIt], sizeof(msg.sequence));
					readIt += sizeof(msg.sequence);
					memcpy(&msg.bufferSize, &m_receiveBuffer[readIt], sizeof(msg.bufferSize));
					readIt += sizeof(msg.bufferSize);

					if ((receivedBytes - readIt) < msg.bufferSize)
					{
						break;
					}

					memcpy(&msg.buffer, &m_receiveBuffer[readIt], msg.bufferSize);
					readIt += msg.bufferSize;

					OnFrame(msg);

					success = true;
				}

				break;
			}
		}

		if (!success)
		{
			break;
		}
	}
}

void Networking::OnACK(const ACK& acMessage)
{
#if GAME_SERVER
#else
#endif
}

void Networking::OnConnectionRequest(const ConnectionRequest& acMessage)
{
#if GAME_SERVER
#else
#endif
}

void Networking::OnConnectionResponse(const ConnectionResponse& acMessage)
{
#if GAME_SERVER
#else
#endif
}

void Networking::OnPlay2DSound(const Play2DSound& acMessage)
{
#if GAME_SERVER
#else
#endif
}

void Networking::OnPlay3DSound(const Play3DSound& acMessage)
{
#if GAME_SERVER
#else
#endif
}

void Networking::OnPlayerInput(const PlayerInput& acMessage)
{
#if GAME_SERVER

#else
#endif
}

void Networking::OnFrame(const Frame& acMessage)
{
#if GAME_SERVER
#else
#endif
}
