#pragma once

#include "Player.h"
#include "Leadwerks.h"

#include <vector>

using namespace Leadwerks;

class AIPlayer : public Player
{
public:
	AIPlayer(Unit* apParent, std::vector<Unit*>* apEnemies, std::vector<Entity*>* apObjectives);
	~AIPlayer();

	virtual void OnRespawn() override;

	virtual Player::EPlayerType GetType() override;

protected:
	virtual void CollectInput() override;

	void UpdateWaypoint();
	void KillEnemies();
	void ManageWeapon();

	enum EObjectiveState
	{
		Finding,
		MovingTo,
		Camping,
	};
	EObjectiveState m_currentObjectiveState;
	std::vector<Vec3> m_pathToObjective;
	size_t m_currentCheckpoint;
	float m_previousDistance;

	static constexpr float s_cFindPathInterval = 1.5f;
	float m_findPathInterval;

	static constexpr float s_cFindDifferentPathInterval = 3.f;
	float m_findDifferentPathInterval;

	static constexpr float s_cDistanceCheckInterval = .015f;
	float m_distanceInterval;

	static constexpr float s_cMaxCampingTime = 5.0f;
	float m_currentCampingTime;

	std::vector<Entity*>* m_pObjectives;

	std::vector<Unit*>* m_pEnemies;
	size_t m_unitToCheck;
	float m_timeTillCheck;
	static constexpr float s_cMaxTimeTillCheck = .5f;
	Unit* m_pCurrentTarget;

	Entity* m_pSelf;
	Entity* m_pSelfHead;
	Entity* m_pPivot;

};