#pragma once

#include <cstdint>
#include <string>

enum class ENetworkMessages : uint8_t
{
	ACK,
	ConnectionRequest,
	ConnectionResponse,
	Play2DSound,
	Play3DSound,
	PlayerInput,
	Frame,
};

struct NetworkMessage
{
	NetworkMessageHeader header;
	NetworkMessageData data;
};

struct NetworkMessageHeader
{
	ENetworkMessages opcode;
	uint32_t messageSize;
};

struct NetworkMessageData
{
};

struct ACK : public NetworkMessageData
{
	uint8_t sequenceID;
};

struct ConnectionRequest : public NetworkMessageData
{
};

enum class EConnectionResponse : uint8_t
{
	NoResponse,
	Accepted,
	GameIsFull,
};

struct ConnectionResponse : public NetworkMessageData
{
	EConnectionResponse response;
};

enum class ESoundID : uint8_t
{
	AR_Shot,
	AR_Reload,
	Respawn,
	Death,
	Footstep,
	Crouching,
	Victory,
	Defeat,
	Hitmarker,
	Impact,
};

struct Play2DSound : public NetworkMessageData
{
	ESoundID soundID;
};

struct Play3DSound : public NetworkMessageData
{
	ESoundID soundID;
	float posX;
	float posY;
	float posZ;
};

struct PlayerInput : public NetworkMessageData
{
	float move;
	float strafe;
	float rotX;
	float rotY;
	uint8_t crouch;
	uint8_t shoot;
	uint8_t aimDownSight;
	uint8_t reload;
};

struct Frame : public NetworkMessageData
{
	uint8_t sequence;
	uint16_t bufferSize;
	std::string buffer;
};
