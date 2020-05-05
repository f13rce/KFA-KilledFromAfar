/* This file was automatically generated. Do not modify! */
/* For more info, find protocol.txt / codegen.py or contact f13rce#1313 */

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <vector>
#include <string>
#include <memory>

#include "data\messages\MessageTypes.h"

#include "data\stream\DataStreamReader.h"
#include "data\stream\DataStreamWriter.h"

namespace Net13
{
namespace Messages
{

class Message
{
public:
	Message::Message();
	Message::~Message();

	virtual void Serialize(Net13::DataStreamWriter& aWriter){}
	virtual void Deserialize(Net13::DataStreamReader& aReader){}
	virtual void Execute(){}

	virtual uint16_t GetMessageSize(){ return 0; }
	virtual uint8_t GetMessageOpcode(){ return 0; }
	virtual const char* GetMessageName() { return "Message"; }

	virtual std::shared_ptr<Message> CreateInstance();

	Messages::EMessageType m_MessageType;
	uint8_t m_SequenceID; // If this is 0, there is no sequence
	uint16_t m_ConnectionID; // ConnectionID from the sender
};

class MessageHeader : public Message
{
public:
	MessageHeader();
	~MessageHeader();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint16_t size;
	uint8_t type;
	uint8_t opcode;
	uint8_t sequenceID;
	uint16_t connectionID;
};
extern void MessageHeader_Receive(const Messages::MessageHeader& acMessage);

class ACK : public Message
{
public:
	ACK();
	~ACK();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint16_t connectionID;
	uint8_t id;
};
extern void ACK_Receive(const Messages::ACK& acMessage);

class AcknowledgeACK : public Message
{
public:
	AcknowledgeACK();
	~AcknowledgeACK();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint16_t connectionID;
	uint8_t id;
};
extern void AcknowledgeACK_Receive(const Messages::AcknowledgeACK& acMessage);

class ConnectionRequest : public Message
{
public:
	ConnectionRequest();
	~ConnectionRequest();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

};
extern void ConnectionRequest_Receive(const Messages::ConnectionRequest& acMessage);

class ConnectionResponse : public Message
{
public:
	ConnectionResponse();
	~ConnectionResponse();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t response;
	uint16_t connectionID;
};
extern void ConnectionResponse_Receive(const Messages::ConnectionResponse& acMessage);

class Heartbeat : public Message
{
public:
	Heartbeat();
	~Heartbeat();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint16_t connectionID;
	uint8_t id;
	uint32_t ping;
};
extern void Heartbeat_Receive(const Messages::Heartbeat& acMessage);

class Disconnect : public Message
{
public:
	Disconnect();
	~Disconnect();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t reason;
	uint16_t connectionID;
};
extern void Disconnect_Receive(const Messages::Disconnect& acMessage);

class JoinGameRequest : public Message
{
public:
	JoinGameRequest();
	~JoinGameRequest();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	std::string name;
};
extern void JoinGameRequest_Receive(const Messages::JoinGameRequest& acMessage);

class JoinGameResponse : public Message
{
public:
	JoinGameResponse();
	~JoinGameResponse();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t response;
};
extern void JoinGameResponse_Receive(const Messages::JoinGameResponse& acMessage);

class ChatMessage : public Message
{
public:
	ChatMessage();
	~ChatMessage();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	std::string message;
};
extern void ChatMessage_Receive(const Messages::ChatMessage& acMessage);

class SystemMessage : public Message
{
public:
	SystemMessage();
	~SystemMessage();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	std::string message;
};
extern void SystemMessage_Receive(const Messages::SystemMessage& acMessage);

class Play2DSound : public Message
{
public:
	Play2DSound();
	~Play2DSound();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t soundID;
};
extern void Play2DSound_Receive(const Messages::Play2DSound& acMessage);

class Play3DSound : public Message
{
public:
	Play3DSound();
	~Play3DSound();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t soundID;
	float posX;
	float posY;
	float posZ;
};
extern void Play3DSound_Receive(const Messages::Play3DSound& acMessage);

class PlayerInput : public Message
{
public:
	PlayerInput();
	~PlayerInput();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	float move;
	float strafe;
	float rotX;
	float rotY;
	uint8_t crouch;
	uint8_t shoot;
	uint8_t aimDownSight;
	uint8_t reload;
};
extern void PlayerInput_Receive(const Messages::PlayerInput& acMessage);

class Frame : public Message
{
public:
	Frame();
	~Frame();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t sequence;
	uint32_t originalSize;
	std::string buffer;
};
extern void Frame_Receive(const Messages::Frame& acMessage);

class PixelColorID : public Message
{
public:
	PixelColorID();
	~PixelColorID();

	void Serialize(Net13::DataStreamWriter& aWriter);
	void Deserialize(Net13::DataStreamReader& aReader);
	void Execute();

	uint16_t GetMessageSize();
	uint8_t GetMessageOpcode();
	const char* GetMessageName();

	std::shared_ptr<Message> CreateInstance();

	uint8_t id;
	float r;
	float g;
	float b;
};
extern void PixelColorID_Receive(const Messages::PixelColorID& acMessage);

} // namespace Messages
} // namespace Net13

#endif
