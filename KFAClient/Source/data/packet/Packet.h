#ifndef _PACKET_H_
#define _PACKET_H_

#include <list>
#include <memory>

#include "data\stream\DataStream.h"
#include "data\messages\NetcodeMessages.h"

namespace Net13
{
	class Message;

	class Packet
	{
	public:
		enum
		{
			kMaxPacketSize = UINT16_MAX,
		};

		Packet();
		~Packet();

		void AddMessage(std::shared_ptr<Messages::Message> apMessage);
		void RemoveMessage(std::shared_ptr<Messages::Message> apMessage);

		void Serialize();
		void Deserialize();

		void Encrypt();
		void Decrypt();

		void SetData(const char* acData, uint16_t aLength);
		const char* GetData() const;
		uint16_t GetMessagesSize() const;
		uint16_t GetDataSize() const;
		uint32_t GetMessageCount() const;

		DataStream& GetDataStream();

	private:
		DataStream m_DataStream;
		
		std::list<std::shared_ptr<Messages::Message>> m_Messages;
	};
}

#endif
