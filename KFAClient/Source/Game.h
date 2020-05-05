#pragma once

#include "NetworkManager.h"
#include "PixelCompressor.h"
#include "GameChat.h"

#include "Leadwerks.h"
using namespace Leadwerks;

#include <memory>

#define MOVEMENT_MIN -1.f
#define MOVEMENT_MAX 1.f

#define ROTY_MIN -85.f
#define ROTY_MAX 85.f

class Game
{
public:
	Game();
	~Game();

	void Init();

	void Connect();

	bool Update();
	void Draw();

	GameChat* GetGameChat();

	void OnJoinSuccessful();
	void OnJoinUnsuccessful();

	void OnPixelColorIDReceived(const Net13::Messages::PixelColorID& acMessage);
	void OnFrameReceived(uint8_t aSequence, const std::string& acFrameBuffer, uint32_t aOriginalProtocolSize);

private:
	void RegisterCustomNetworkMessages();
	void UpdateInput();
	bool SequenceIsNewer(uint8_t aSequence);

	Window* m_pWindow;
	Context* m_pContext;
	Texture* m_pFrame;

	Net13::NetworkManager m_networkManager;
	static constexpr uint16_t s_cServerPort = 13337;
	static constexpr uint16_t s_cClientPort = 13338;

	GameChat m_gameChat;

	PixelCompressor m_compressor;
	uint8_t m_frameSequence;

	float m_connectCooldown;

	std::shared_ptr<Net13::Messages::PlayerInput> m_input;
	float m_inputTick;

	bool m_joinedGame;
	bool m_sentJoinGameRequest;

};

extern Game* g_pGame;
extern float g_deltaTime;
