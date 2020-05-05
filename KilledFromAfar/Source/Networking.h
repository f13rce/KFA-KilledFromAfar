#pragma once

#include <cstdint>
#include <vector>

#include "Socket.h"
#include "NetworkMessages.h"

class Networking
{
public:
	Networking();
	~Networking();

	void Update();

private:
	void Send();
	void Receive();

	void OnACK(const ACK& acMessage);
	void OnConnectionRequest(const ConnectionRequest& acMessage);
	void OnConnectionResponse(const ConnectionResponse& acMessage);
	void OnPlay2DSound(const Play2DSound& acMessage);
	void OnPlay3DSound(const Play3DSound& acMessage);
	void OnPlayerInput(const PlayerInput& acMessage);
	void OnFrame(const Frame& acMessage);

	Leadwerks::Socket* m_pSocket;

	std::vector<std::unique_ptr<NetworkMessage>> m_messages;

	static constexpr uint16_t s_cMaxBufferSize = UINT16_MAX;
	std::string m_sendBuffer;
	std::string m_receiveBuffer;

#ifdef GAME_SERVER
	static constexpr uint16_t s_cPort = 13337;
#endif

};