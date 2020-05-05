#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include "systems\NetworkSystem.h"

namespace Net13
{
	class NetworkServer : public NetworkSystem
	{
	public:
		enum
		{
			kDefaultMaxPlayers = 32,
		};

		NetworkServer();
		~NetworkServer();

		void SetMaxPlayers(uint8_t aPlayerCount);

	private:
		void OnConnectionRequest(const Messages::ConnectionRequest& acMessage);
		void OnConnectionResponse(const Messages::ConnectionResponse& acMessage);
		void OnDisconnect(const Messages::Disconnect& acMessage);

		uint8_t m_MaxPlayerCount;
	};
}

#endif
