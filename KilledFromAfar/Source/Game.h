#pragma once

#include "BulletManager.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GameMode.h"
#include "NetworkManager.h"
#include "NetworkPlayer.h"
#include "PixelCompressor.h"

#include <memory>
#include <chrono>

class Game
{
public:
	Game();
	~Game();

	void SetTestTexture(Texture* apTestTexture);

	void Init();

	void Update();

	void PhysicsUpdate();
	void NetworkingUpdate();

	static BulletManager* GetBulletManager();
	static UnitManager* GetUnitManager();

	void OnPlayerJoined(uint16_t aConnectionID, const std::string& acName);
	void OnPlayerLeft(uint16_t aConnectionID);
	bool IsFull();

	static std::vector<NetworkPlayer> s_networkPlayers;
	static constexpr size_t s_cMaxPlayers = 32;
	static constexpr size_t s_cMaxPlayerNameLength = 16;
	static constexpr size_t s_cMaxPlayerChatLength = 64;

	static NetworkPlayer* GetNetworkPlayer(uint16_t aConnectionID);

	void SendPlayerData();

private:
	void RegisterCustomNetworkMessages();

	static BulletManager s_bulletManager;
	static UnitManager s_unitManager;

	std::chrono::time_point<std::chrono::steady_clock> m_previousTime;

	std::unique_ptr<GameMode> m_pGameMode;

	Net13::NetworkManager m_networkManager;
	static constexpr uint16_t s_cServerPort = 13337;

	PixelCompressor m_compressor;
	Texture* m_pTestTexture;

};

extern Game* g_pGame;
