#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Unit.h"

class UnitManager
{
public:
	UnitManager();
	~UnitManager();

	void Init();

	Unit* SpawnUnit(Unit::ETeam aTeam, Player::EPlayerType aPlayerType);
	void RespawnUnit(Unit* apUnit);
	void RemoveUnit(int32_t aUnitID);

	void Update();
	void PhysicsUpdate();

	Unit* GetUnit(Entity* apModel);

protected:
	void FindSpawns();
	void FindObjectives();

	void SpawnAIPlayers();

	int32_t m_idCounter;

	std::vector<std::unique_ptr<Unit>> m_units;

	std::vector<Entity*> m_attackersSpawns;
	std::vector<Entity*> m_defendersSpawns;

	std::vector<Entity*> m_attackersObjectives;
	std::vector<Entity*> m_defendersObjectives;

	std::vector<Unit*> m_attackers;
	std::vector<Unit*> m_defenders;

	static constexpr uint8_t s_cMaxAIPerTeam = 4;

};