#include "Game.h"

#include "TeamDeathmatch.h"
#include "data/messages/NetcodeMessages.h"

#include "Globals.h"

BulletManager Game::s_bulletManager;
UnitManager Game::s_unitManager;
std::vector<NetworkPlayer> Game::s_networkPlayers;

Game* g_pGame = nullptr;

Game::Game()
	: m_pGameMode(std::make_unique<TeamDeathmatch>())
	, m_networkManager(s_cServerPort, true)
	, m_pTestTexture(nullptr)
{
	RegisterCustomNetworkMessages();
	m_networkManager.SetTickRate(144);

	g_pGame = this;
}

Game::~Game()
{
	m_networkManager.Disconnect();
}

void Game::SetTestTexture(Texture* apTestTexture)
{
	m_pTestTexture = apTestTexture;
}

void Game::Init()
{
	m_previousTime = std::chrono::steady_clock::now();

	std::cout << "Starting BulletManager..." << std::endl;
	s_bulletManager.Init();

	std::cout << "Starting UnitManager..." << std::endl;
	s_unitManager.Init();

#if USE_LOCAL_PLAYER
	std::cout << "Spawning local player..." << std::endl;
	auto pUnit = s_unitManager.SpawnUnit(Unit::ETeam::Defenders, Player::EPlayerType::Local);

	NetworkPlayer player(-1, pUnit, "SERVER_PLAYER");

	s_networkPlayers.push_back(player);
#endif

	std::cout << "Finished initializing game!" << std::endl;
}

void Game::Update()
{
	// Delta time
	auto now = std::chrono::steady_clock::now();
	float delta = float(std::chrono::duration_cast<chrono::microseconds>(now - m_previousTime).count());
	g_deltaTime = delta / 1000000.f;
	m_previousTime = now;

	s_unitManager.Update();
	s_bulletManager.Update();
}

void Game::PhysicsUpdate()
{
	s_unitManager.PhysicsUpdate();
}

void Game::NetworkingUpdate()
{
	m_networkManager.Update();
}

BulletManager* Game::GetBulletManager()
{
	return &s_bulletManager;
}

UnitManager* Game::GetUnitManager()
{
	return &s_unitManager;
}

void Game::OnPlayerJoined(uint16_t aConnectionID, const std::string& acName)
{
	auto pUnit = s_unitManager.SpawnUnit(Unit::ETeam::Attackers, Player::EPlayerType::Remote);

	NetworkPlayer player(aConnectionID, pUnit, acName);

	s_networkPlayers.push_back(player);

	// Send pixel colors
	const auto& colors = m_compressor.GetPixelColors();
	const auto& hashes = m_compressor.GetPixelHashes();
	
	for (size_t i = 0; i < colors.size(); ++i)
	{
		auto pMessage = std::make_shared<Net13::Messages::PixelColorID>();
		pMessage->id = hashes[i];
		pMessage->r = colors[i].r;
		pMessage->g = colors[i].g;
		pMessage->b = colors[i].b;
		m_networkManager.SendReliable(pMessage, aConnectionID);
	}
}

void Game::OnPlayerLeft(uint16_t aConnectionID)
{
	for (auto i = 0; i < s_networkPlayers.size(); ++i)
	{
		if (s_networkPlayers[i].GetConnectionID() == aConnectionID)
		{
			s_networkPlayers.erase(s_networkPlayers.begin() + i);
			break;
		}
	}
}

bool Game::IsFull()
{
	return (s_networkPlayers.size() == s_cMaxPlayers);
}

NetworkPlayer* Game::GetNetworkPlayer(uint16_t aConnectionID)
{
	for (size_t i = 0; i < s_networkPlayers.size(); ++i)
	{
		if (s_networkPlayers[i].GetConnectionID() == aConnectionID)
		{
			return &s_networkPlayers[i];
		}
	}

	return nullptr;
}

void Game::SendPlayerData()
{
	for (auto i = 0; i < s_networkPlayers.size(); ++i)
	{
#if USE_LOCAL_PLAYER
		if (s_networkPlayers[i].GetUnit()->GetPlayerType() != Player::EPlayerType::Remote)
		{
			continue;
		}
#endif
		auto pFrameMsg = std::make_shared<Net13::Messages::Frame>();
		pFrameMsg->sequence = s_networkPlayers[i].GetFrameSequence();
		PixelCompressor::CompressionOutput output = m_compressor.Compress(s_networkPlayers[i].GetUnit()->GetRenderTexture());
		pFrameMsg->buffer = output.buffer;
		pFrameMsg->originalSize = output.protocolCount;
		m_networkManager.Send(pFrameMsg);

		/*if (s_networkPlayers[i].GetUnit()->GetPlayerType() == Player::EPlayerType::Remote)
		{
			m_compressor.Decompress(m_pTestTexture, output.buffer, output.protocolCount);
		}*/
	}
}

void Game::RegisterCustomNetworkMessages()
{
	auto pJoinGameRequest = std::make_unique<Net13::Messages::JoinGameRequest>();
	m_networkManager.RegisterMessage(std::move(pJoinGameRequest), pJoinGameRequest->GetMessageOpcode());

	auto pJoinGameResponse = std::make_unique<Net13::Messages::JoinGameResponse>();
	m_networkManager.RegisterMessage(std::move(pJoinGameResponse), pJoinGameResponse->GetMessageOpcode());

	auto pChatMessage = std::make_unique<Net13::Messages::ChatMessage>();
	m_networkManager.RegisterMessage(std::move(pChatMessage), pChatMessage->GetMessageOpcode());

	auto pSystemMessage = std::make_unique<Net13::Messages::SystemMessage>();
	m_networkManager.RegisterMessage(std::move(pSystemMessage), pSystemMessage->GetMessageOpcode());

	auto pPlay2DSound = std::make_unique<Net13::Messages::Play2DSound>();
	m_networkManager.RegisterMessage(std::move(pPlay2DSound), pPlay2DSound->GetMessageOpcode());

	auto pPlay3DSound = std::make_unique<Net13::Messages::Play3DSound>();
	m_networkManager.RegisterMessage(std::move(pPlay3DSound), pPlay3DSound->GetMessageOpcode());

	auto pPlayerInput = std::make_unique<Net13::Messages::PlayerInput>();
	m_networkManager.RegisterMessage(std::move(pPlayerInput), pPlayerInput->GetMessageOpcode());

	auto pFrame = std::make_unique<Net13::Messages::Frame>();
	m_networkManager.RegisterMessage(std::move(pFrame), pFrame->GetMessageOpcode());

	auto pPixelColorID = std::make_unique<Net13::Messages::PixelColorID>();
	m_networkManager.RegisterMessage(std::move(pPixelColorID), pPixelColorID->GetMessageOpcode());
}

void Net13::Messages::JoinGameRequest_Receive(const Net13::Messages::JoinGameRequest& acMessage)
{
	std::cout << "Player join request..." << std::endl;

	auto pPlayer = g_pGame->GetNetworkPlayer(acMessage.m_ConnectionID);
	if (pPlayer != nullptr)
	{
		std::cout << "Player didn't get the memo yet I suppose" << std::endl;
		// Notify player that he's already in
		//auto pMessage = std::make_shared<Net13::Messages::JoinGameResponse>();
		//pMessage->response = 1;
		//Net13::g_Network->Send(pMessage, acMessage.m_ConnectionID);
		return;
	}

	if (g_pGame->IsFull())
	{
		std::cout << "Server is full! Sorry bud." << std::endl;

		// Notify player
		auto pMessage = std::make_shared<Net13::Messages::JoinGameResponse>();
		pMessage->response = 2;
		Net13::g_Network->SendReliable(pMessage, acMessage.m_ConnectionID);
	}
	else
	{
		// Sanitize name
		std::string playerName = acMessage.name;
		if (playerName.size() > Game::s_cMaxPlayerNameLength)
		{
			playerName.resize(Game::s_cMaxPlayerNameLength);
		}

		std::cout << "Player " << playerName << " has joined the game!" << std::endl;

		// Notify player
		auto pMessage = std::make_shared<Net13::Messages::JoinGameResponse>();
		pMessage->response = 1;
		Net13::g_Network->SendReliable(pMessage, acMessage.m_ConnectionID);

		// Callback
		g_pGame->OnPlayerJoined(acMessage.m_ConnectionID, playerName);
	}
}

void Net13::Messages::JoinGameResponse_Receive(const Net13::Messages::JoinGameResponse& acMessage)
{
}

void Net13::Messages::ChatMessage_Receive(const Net13::Messages::ChatMessage& acMessage)
{
	auto pPlayer = Game::GetNetworkPlayer(acMessage.m_ConnectionID);
	if (!pPlayer)
	{
		return;
	}

	auto pMessage = std::make_shared<Net13::Messages::ChatMessage>();

	// Build chat message
	std::string playersChatMsg = acMessage.message;
	if (playersChatMsg.size() > Game::s_cMaxPlayerChatLength)
	{
		playersChatMsg.resize(Game::s_cMaxPlayerChatLength);
	}
	std::string chatMsg = "[" + pPlayer->GetName() + "]: " + playersChatMsg;

	// Send to others
	for (size_t i = 0; i < Game::s_networkPlayers.size(); ++i)
	{
		if (Game::s_networkPlayers[i].GetConnectionID() != acMessage.m_ConnectionID)
		{
			Net13::g_Network->SendReliable(pMessage, Game::s_networkPlayers[i].GetConnectionID());
		}
	}
}

void Net13::Messages::SystemMessage_Receive(const Net13::Messages::SystemMessage& acMessage)
{
}

void Net13::Messages::Play2DSound_Receive(const Net13::Messages::Play2DSound& acMessage)
{
}

void Net13::Messages::Play3DSound_Receive(const Net13::Messages::Play3DSound& acMessage)
{
}

void Net13::Messages::PlayerInput_Receive(const Net13::Messages::PlayerInput& acMessage)
{
	NetworkPlayer* pPlayer = Game::GetNetworkPlayer(acMessage.m_ConnectionID);
	if (!pPlayer)
	{
		return;
	}

	Player::Input input;
	input.aimDownSight = acMessage.aimDownSight;
	input.crouch = acMessage.crouch;
	input.move = acMessage.move;
	input.reload = acMessage.reload;
	input.rotX = acMessage.rotX;
	input.rotY = acMessage.rotY;
	input.shoot = acMessage.shoot;
	input.strafe = acMessage.strafe;

	//std::cout << "Received input from Player " << pPlayer->GetName() << std::endl;
	//std::cout << "\tMove: " << input.move << " Shoot: " << input.shoot << " RotX: " << input.rotX << " RotY: " << input.rotY << std::endl;

	pPlayer->GetUnit()->GetPlayer()->SetInput(input);
}

void Net13::Messages::Frame_Receive(const Net13::Messages::Frame& acMessage)
{
}

void Net13::Messages::PixelColorID_Receive(const Net13::Messages::PixelColorID& acMessage)
{
}
