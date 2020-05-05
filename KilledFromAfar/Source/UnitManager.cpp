#include "UnitManager.h"
#include "Globals.h"

#include "AIPlayer.h"
#include "RemotePlayer.h"
#if USE_LOCAL_PLAYER
#include "LocalPlayer.h"
#endif

#include "Game.h"

UnitManager::UnitManager()
	: m_idCounter(0)
{
}

UnitManager::~UnitManager()
{
}

void UnitManager::Init()
{
	FindSpawns();
	FindObjectives();

	SpawnAIPlayers();
}

Unit* UnitManager::SpawnUnit(Unit::ETeam aTeam, Player::EPlayerType aPlayerType)
{
	m_units.push_back(std::make_unique<Unit>(m_idCounter));
	m_idCounter++;

	auto* pUnit = m_units.back().get();
	pUnit->SetTeam(aTeam);

	std::unique_ptr<Player> pPlayer;
	switch (aPlayerType)
	{
		case Player::EPlayerType::AI:
		{
			//, std::vector<Unit*>* apEnemies, std::vector<Entity*>* apObjectives
			pPlayer = std::make_unique<AIPlayer>(
				pUnit,
				aTeam == Unit::ETeam::Attackers ? &m_defenders : &m_attackers,
				aTeam == Unit::ETeam::Attackers ? &m_attackersObjectives : &m_defendersObjectives
				);
			break;
		}
		
		case Player::EPlayerType::Remote:
		{
			pPlayer = std::make_unique<RemotePlayer>(pUnit);
			break;
		}

#if USE_LOCAL_PLAYER
		case Player::EPlayerType::Local:
		{
			pPlayer = std::make_unique<LocalPlayer>(pUnit);
			break;
		}
#endif
	}

	if (!pPlayer)
	{
		m_units.pop_back();
		return nullptr;
	}

	pUnit->SetPlayer(std::move(pPlayer));
	pPlayer = nullptr;

	switch (aTeam)
	{
		case Unit::ETeam::Attackers:
		{
			m_attackers.push_back(pUnit);
			pUnit->Respawn(m_attackersSpawns[rand() % m_attackersSpawns.size()]);
			break;
		}

		case Unit::ETeam::Defenders:
		{
			m_defenders.push_back(pUnit);
			pUnit->Respawn(m_defendersSpawns[rand() % m_defendersSpawns.size()]);
			break;
		}
	}

	return pUnit;
}

void UnitManager::RespawnUnit(Unit* apUnit)
{
	if (apUnit->GetTeam() == Unit::ETeam::Attackers)
	{
		apUnit->Respawn(m_attackersSpawns[rand() % m_attackersSpawns.size()]);
	}
	else
	{
		apUnit->Respawn(m_defendersSpawns[rand() % m_defendersSpawns.size()]);
	}
}

void UnitManager::RemoveUnit(int32_t aUnitID)
{
	for (auto i = 0; i < m_units.size(); ++i)
	{
		if (m_units[i]->GetID() == aUnitID)
		{
			m_units.erase(m_units.begin() + i);
		}
	}
}

void UnitManager::Update()
{
	for (const auto& pUnit : m_units)
	{
		pUnit->Update();
	}
}

void UnitManager::PhysicsUpdate()
{
	for (const auto& pUnit : m_units)
	{
		pUnit->PhysicsUpdate();

		if (pUnit->GetPhysicsModel()->GetPosition().y <= -25.f)
		{
			pUnit->Respawn(
				pUnit->GetTeam() == Unit::ETeam::Attackers ?
				m_attackersSpawns[rand() % m_attackersSpawns.size()] :
				m_defendersSpawns[rand() % m_defendersSpawns.size()]
			);
		}
	}
}

Unit* UnitManager::GetUnit(Entity* apModel)
{
	for (const auto& pUnit : m_units)
	{
		if (pUnit->GetAppearanceModel() == apModel || pUnit->GetPhysicsModel() == apModel)
		{
			return pUnit.get();
		}
	}

	return nullptr;
}

void UnitManager::FindSpawns()
{
	auto entities = g_pWorld->entities;

	// Attackers
	for (auto pEntity : entities)
	{
		if (!pEntity) continue;

		if (pEntity->GetKeyValue(NAME_KEY) == "Spawn_Attackers")
		{
			m_attackersSpawns.push_back(pEntity);
		}
	}

	// Defenders
	for (auto pEntity : entities)
	{
		if (!pEntity) continue;

		if (pEntity->GetKeyValue(NAME_KEY) == "Spawn_Defenders")
		{
			m_defendersSpawns.push_back(pEntity);
		}
	}
}

void UnitManager::FindObjectives()
{
	auto entities = g_pWorld->entities;

	// Attackers
	for (auto pEntity : entities)
	{
		if (!pEntity) continue;

		if (pEntity->GetKeyValue(NAME_KEY) == "AttackerObjective")
		{
			m_attackersObjectives.push_back(pEntity);
		}
	}

	// Defenders
	for (auto pEntity : entities)
	{
		if (!pEntity) continue;

		if (pEntity->GetKeyValue(NAME_KEY) == "DefenderObjective")
		{
			m_defendersObjectives.push_back(pEntity);
		}
	}
}

void UnitManager::SpawnAIPlayers()
{
	for (auto i = 0; i < s_cMaxAIPerTeam; ++i)
	{
		SpawnUnit(Unit::ETeam::Attackers, Player::EPlayerType::AI);
	}

	for (auto i = 0; i < s_cMaxAIPerTeam; ++i)
	{
		SpawnUnit(Unit::ETeam::Defenders, Player::EPlayerType::AI);
	}
}
