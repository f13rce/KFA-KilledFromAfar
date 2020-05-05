/* This file was automatically generated. Do not modify! */
/* For more info, find protocol.txt / codegen.py or contact @Ivar Slotboom */

#include "data\messages\NetcodeMessages.h"

namespace Net13
{
namespace Messages
{

Message::Message() : m_SequenceID(0), m_ConnectionID(0) {}
Message::~Message() {}

std::shared_ptr<Message> Message::CreateInstance()
{
	return std::move(std::make_unique<Message>());
}

MessageHeader::MessageHeader()
{
}

MessageHeader::~MessageHeader()
{
}

void MessageHeader::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(0)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(size);
	aWriter.Write(type);
	aWriter.Write(opcode);
	aWriter.Write(sequenceID);
	aWriter.Write(connectionID);
}

void MessageHeader::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(size);
	aReader.Read(type);
	aReader.Read(opcode);
	aReader.Read(sequenceID);
	aReader.Read(connectionID);
}

void MessageHeader::Execute()
{
	MessageHeader_Receive(*this);
}

uint16_t MessageHeader::GetMessageSize()
{
	return uint16_t(13);
}

uint8_t MessageHeader::GetMessageOpcode()
{
	return 0;
}

const char* MessageHeader::GetMessageName()
{
	return "MessageHeader";
}

std::shared_ptr<Message> MessageHeader::CreateInstance()
{
	return std::move(std::make_unique<MessageHeader>());
}

ACK::ACK()
{
}

ACK::~ACK()
{
}

void ACK::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(1)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(connectionID);
	aWriter.Write(id);
}

void ACK::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(connectionID);
	aReader.Read(id);
}

void ACK::Execute()
{
	ACK_Receive(*this);
}

uint16_t ACK::GetMessageSize()
{
	return uint16_t(9);
}

uint8_t ACK::GetMessageOpcode()
{
	return 1;
}

const char* ACK::GetMessageName()
{
	return "ACK";
}

std::shared_ptr<Message> ACK::CreateInstance()
{
	return std::move(std::make_unique<ACK>());
}

AcknowledgeACK::AcknowledgeACK()
{
}

AcknowledgeACK::~AcknowledgeACK()
{
}

void AcknowledgeACK::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(2)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(connectionID);
	aWriter.Write(id);
}

void AcknowledgeACK::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(connectionID);
	aReader.Read(id);
}

void AcknowledgeACK::Execute()
{
	AcknowledgeACK_Receive(*this);
}

uint16_t AcknowledgeACK::GetMessageSize()
{
	return uint16_t(9);
}

uint8_t AcknowledgeACK::GetMessageOpcode()
{
	return 2;
}

const char* AcknowledgeACK::GetMessageName()
{
	return "AcknowledgeACK";
}

std::shared_ptr<Message> AcknowledgeACK::CreateInstance()
{
	return std::move(std::make_unique<AcknowledgeACK>());
}

ConnectionRequest::ConnectionRequest()
{
}

ConnectionRequest::~ConnectionRequest()
{
}

void ConnectionRequest::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(3)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
}

void ConnectionRequest::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
}

void ConnectionRequest::Execute()
{
	ConnectionRequest_Receive(*this);
}

uint16_t ConnectionRequest::GetMessageSize()
{
	return uint16_t(6);
}

uint8_t ConnectionRequest::GetMessageOpcode()
{
	return 3;
}

const char* ConnectionRequest::GetMessageName()
{
	return "ConnectionRequest";
}

std::shared_ptr<Message> ConnectionRequest::CreateInstance()
{
	return std::move(std::make_unique<ConnectionRequest>());
}

ConnectionResponse::ConnectionResponse()
{
}

ConnectionResponse::~ConnectionResponse()
{
}

void ConnectionResponse::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(4)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(response);
	aWriter.Write(connectionID);
}

void ConnectionResponse::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(response);
	aReader.Read(connectionID);
}

void ConnectionResponse::Execute()
{
	ConnectionResponse_Receive(*this);
}

uint16_t ConnectionResponse::GetMessageSize()
{
	return uint16_t(9);
}

uint8_t ConnectionResponse::GetMessageOpcode()
{
	return 4;
}

const char* ConnectionResponse::GetMessageName()
{
	return "ConnectionResponse";
}

std::shared_ptr<Message> ConnectionResponse::CreateInstance()
{
	return std::move(std::make_unique<ConnectionResponse>());
}

Heartbeat::Heartbeat()
{
}

Heartbeat::~Heartbeat()
{
}

void Heartbeat::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(5)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(connectionID);
	aWriter.Write(id);
	aWriter.Write(ping);
}

void Heartbeat::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(connectionID);
	aReader.Read(id);
	aReader.Read(ping);
}

void Heartbeat::Execute()
{
	Heartbeat_Receive(*this);
}

uint16_t Heartbeat::GetMessageSize()
{
	return uint16_t(13);
}

uint8_t Heartbeat::GetMessageOpcode()
{
	return 5;
}

const char* Heartbeat::GetMessageName()
{
	return "Heartbeat";
}

std::shared_ptr<Message> Heartbeat::CreateInstance()
{
	return std::move(std::make_unique<Heartbeat>());
}

Disconnect::Disconnect()
{
}

Disconnect::~Disconnect()
{
}

void Disconnect::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(6)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(reason);
	aWriter.Write(connectionID);
}

void Disconnect::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(reason);
	aReader.Read(connectionID);
}

void Disconnect::Execute()
{
	Disconnect_Receive(*this);
}

uint16_t Disconnect::GetMessageSize()
{
	return uint16_t(9);
}

uint8_t Disconnect::GetMessageOpcode()
{
	return 6;
}

const char* Disconnect::GetMessageName()
{
	return "Disconnect";
}

std::shared_ptr<Message> Disconnect::CreateInstance()
{
	return std::move(std::make_unique<Disconnect>());
}

JoinGameRequest::JoinGameRequest()
{
}

JoinGameRequest::~JoinGameRequest()
{
}

void JoinGameRequest::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(7)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(name);
}

void JoinGameRequest::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(name);
}

void JoinGameRequest::Execute()
{
	JoinGameRequest_Receive(*this);
}

uint16_t JoinGameRequest::GetMessageSize()
{
	return uint16_t(6 + name.size() + 2);
}

uint8_t JoinGameRequest::GetMessageOpcode()
{
	return 7;
}

const char* JoinGameRequest::GetMessageName()
{
	return "JoinGameRequest";
}

std::shared_ptr<Message> JoinGameRequest::CreateInstance()
{
	return std::move(std::make_unique<JoinGameRequest>());
}

JoinGameResponse::JoinGameResponse()
{
}

JoinGameResponse::~JoinGameResponse()
{
}

void JoinGameResponse::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(8)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(response);
}

void JoinGameResponse::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(response);
}

void JoinGameResponse::Execute()
{
	JoinGameResponse_Receive(*this);
}

uint16_t JoinGameResponse::GetMessageSize()
{
	return uint16_t(7);
}

uint8_t JoinGameResponse::GetMessageOpcode()
{
	return 8;
}

const char* JoinGameResponse::GetMessageName()
{
	return "JoinGameResponse";
}

std::shared_ptr<Message> JoinGameResponse::CreateInstance()
{
	return std::move(std::make_unique<JoinGameResponse>());
}

ChatMessage::ChatMessage()
{
}

ChatMessage::~ChatMessage()
{
}

void ChatMessage::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(9)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(message);
}

void ChatMessage::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(message);
}

void ChatMessage::Execute()
{
	ChatMessage_Receive(*this);
}

uint16_t ChatMessage::GetMessageSize()
{
	return uint16_t(6 + message.size() + 2);
}

uint8_t ChatMessage::GetMessageOpcode()
{
	return 9;
}

const char* ChatMessage::GetMessageName()
{
	return "ChatMessage";
}

std::shared_ptr<Message> ChatMessage::CreateInstance()
{
	return std::move(std::make_unique<ChatMessage>());
}

SystemMessage::SystemMessage()
{
}

SystemMessage::~SystemMessage()
{
}

void SystemMessage::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(10)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(message);
}

void SystemMessage::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(message);
}

void SystemMessage::Execute()
{
	SystemMessage_Receive(*this);
}

uint16_t SystemMessage::GetMessageSize()
{
	return uint16_t(6 + message.size() + 2);
}

uint8_t SystemMessage::GetMessageOpcode()
{
	return 10;
}

const char* SystemMessage::GetMessageName()
{
	return "SystemMessage";
}

std::shared_ptr<Message> SystemMessage::CreateInstance()
{
	return std::move(std::make_unique<SystemMessage>());
}

Play2DSound::Play2DSound()
{
}

Play2DSound::~Play2DSound()
{
}

void Play2DSound::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(11)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(soundID);
}

void Play2DSound::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(soundID);
}

void Play2DSound::Execute()
{
	Play2DSound_Receive(*this);
}

uint16_t Play2DSound::GetMessageSize()
{
	return uint16_t(7);
}

uint8_t Play2DSound::GetMessageOpcode()
{
	return 11;
}

const char* Play2DSound::GetMessageName()
{
	return "Play2DSound";
}

std::shared_ptr<Message> Play2DSound::CreateInstance()
{
	return std::move(std::make_unique<Play2DSound>());
}

Play3DSound::Play3DSound()
{
}

Play3DSound::~Play3DSound()
{
}

void Play3DSound::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(12)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(soundID);
	aWriter.Write(posX);
	aWriter.Write(posY);
	aWriter.Write(posZ);
}

void Play3DSound::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(soundID);
	aReader.Read(posX);
	aReader.Read(posY);
	aReader.Read(posZ);
}

void Play3DSound::Execute()
{
	Play3DSound_Receive(*this);
}

uint16_t Play3DSound::GetMessageSize()
{
	return uint16_t(19);
}

uint8_t Play3DSound::GetMessageOpcode()
{
	return 12;
}

const char* Play3DSound::GetMessageName()
{
	return "Play3DSound";
}

std::shared_ptr<Message> Play3DSound::CreateInstance()
{
	return std::move(std::make_unique<Play3DSound>());
}

PlayerInput::PlayerInput()
{
}

PlayerInput::~PlayerInput()
{
}

void PlayerInput::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(13)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(move);
	aWriter.Write(strafe);
	aWriter.Write(rotX);
	aWriter.Write(rotY);
	aWriter.Write(crouch);
	aWriter.Write(shoot);
	aWriter.Write(aimDownSight);
	aWriter.Write(reload);
}

void PlayerInput::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(move);
	aReader.Read(strafe);
	aReader.Read(rotX);
	aReader.Read(rotY);
	aReader.Read(crouch);
	aReader.Read(shoot);
	aReader.Read(aimDownSight);
	aReader.Read(reload);
}

void PlayerInput::Execute()
{
	PlayerInput_Receive(*this);
}

uint16_t PlayerInput::GetMessageSize()
{
	return uint16_t(26);
}

uint8_t PlayerInput::GetMessageOpcode()
{
	return 13;
}

const char* PlayerInput::GetMessageName()
{
	return "PlayerInput";
}

std::shared_ptr<Message> PlayerInput::CreateInstance()
{
	return std::move(std::make_unique<PlayerInput>());
}

Frame::Frame()
{
}

Frame::~Frame()
{
}

void Frame::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(14)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(sequence);
	aWriter.Write(originalSize);
	aWriter.Write(buffer);
}

void Frame::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(sequence);
	aReader.Read(originalSize);
	aReader.Read(buffer);
}

void Frame::Execute()
{
	Frame_Receive(*this);
}

uint16_t Frame::GetMessageSize()
{
	return uint16_t(11 + buffer.size() + 2);
}

uint8_t Frame::GetMessageOpcode()
{
	return 14;
}

const char* Frame::GetMessageName()
{
	return "Frame";
}

std::shared_ptr<Message> Frame::CreateInstance()
{
	return std::move(std::make_unique<Frame>());
}

PixelColorID::PixelColorID()
{
}

PixelColorID::~PixelColorID()
{
}

void PixelColorID::Serialize(Net13::DataStreamWriter& aWriter)
{
	// Serialize header
	aWriter.Write(GetMessageSize()); // message size
	aWriter.Write(uint8_t(15)); // opcode
	aWriter.Write(m_SequenceID); // sequence ID
	aWriter.Write(m_ConnectionID); // connection ID

	// Serialize member variables
	aWriter.Write(id);
	aWriter.Write(r);
	aWriter.Write(g);
	aWriter.Write(b);
}

void PixelColorID::Deserialize(Net13::DataStreamReader& aReader)
{
	// Deserialize member variables
	aReader.Read(id);
	aReader.Read(r);
	aReader.Read(g);
	aReader.Read(b);
}

void PixelColorID::Execute()
{
	PixelColorID_Receive(*this);
}

uint16_t PixelColorID::GetMessageSize()
{
	return uint16_t(19);
}

uint8_t PixelColorID::GetMessageOpcode()
{
	return 15;
}

const char* PixelColorID::GetMessageName()
{
	return "PixelColorID";
}

std::shared_ptr<Message> PixelColorID::CreateInstance()
{
	return std::move(std::make_unique<PixelColorID>());
}

} // namespace Messages
} // namespace Net13

