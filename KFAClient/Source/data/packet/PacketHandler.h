#ifndef _PACKETHANDLER_H_
#define _PACKETHANDLER_H_

#include "Packet.h"

namespace Net13
{
	class PacketHandler
	{
	public:
		enum
		{
			kMaxMessages = 255,
		};

		PacketHandler();
		~PacketHandler();

		void RegisterMessage(std::unique_ptr<Messages::Message> apMessage, uint8_t aOpCode);

		void ProcessPacket(Packet& aPacket);

	private:
		void RegisterDefaultMessages();
		void ProcessMessage();

		std::unique_ptr<Messages::Message> m_Messages[kMaxMessages];
	};
}

#endif