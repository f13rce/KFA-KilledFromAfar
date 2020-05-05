#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <functional>

#include "data\messages\NetcodeMessages.h"

namespace Net13
{
	class NetworkSystem
	{
	public:
		NetworkSystem();
		~NetworkSystem();

		virtual void OnConnectionRequest(const Messages::ConnectionRequest& acMessage) = 0;
		virtual void OnConnectionResponse(const Messages::ConnectionResponse& acMessage) = 0;
		virtual void OnDisconnect(const Messages::Disconnect& acMessage) = 0;

		void RegisterOnConnectionResultCallback(std::function<void(Net13::Messages::EConnectionResult)> aCallback);
		void RegisterOnDisconnectCallback(std::function<void(Net13::Messages::EDisconnectReason)> aCallback);
		void RegisterOnPlayerAddCallback(std::function<void(uint16_t)> aCallback);
		void RegisterOnPlayerDisconnectCallback(std::function<void(uint16_t, Net13::Messages::EDisconnectReason)> aCallback);

		bool IsServer() const;

	protected:
		bool m_IsServer : 1;

		// Gameplay callbacks
		std::function<void(Net13::Messages::EConnectionResult)> m_OnConnectionResult;
		std::function<void(Net13::Messages::EDisconnectReason)> m_OnDisconnect;
		std::function<void(uint16_t)> m_OnPlayerAdd;
		std::function<void(uint16_t, Net13::Messages::EDisconnectReason)> m_OnPlayerDisconnect;
	};
}

#endif
