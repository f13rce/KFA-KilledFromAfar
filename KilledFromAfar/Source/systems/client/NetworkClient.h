#ifndef _NETWORKCLIENT_H_
#define _NETWORKCLIENT_H_

#include "systems\NetworkSystem.h"

namespace Net13
{
	class NetworkClient : public NetworkSystem
	{
	public:
		NetworkClient();
		~NetworkClient();

	private:
		void OnConnectionRequest(const Messages::ConnectionRequest& acMessage);
		void OnConnectionResponse(const Messages::ConnectionResponse& acMessage);
		void OnDisconnect(const Messages::Disconnect& acMessage);
	};
}

#endif
