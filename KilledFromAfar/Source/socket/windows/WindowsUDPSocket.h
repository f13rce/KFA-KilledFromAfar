#ifndef _WINDOWSUDPSOCKET_H_
#define _WINDOWSUDPSOCKET_H_

#if defined(_WIN64) || defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#include "socket\UDPSocket.h"
#pragma comment(lib, "Ws2_32.lib")

namespace Net13
{
	class WindowsUDPSocket : public UDPSocket
	{
	public:
		WindowsUDPSocket(PacketHandler* apPacketHandler, uint16_t aPort, uint8_t aTickRate);
		~WindowsUDPSocket();

	private:
		void Bind(uint16_t aPort);
		void Send();
		void Receive();

		SOCKET m_Socket;
	};
}
#else
namespace Net13
{
	class WindowsUDPSocket;
}
#endif

#endif
