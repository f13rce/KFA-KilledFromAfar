#include "AIPlayer.h"
#include "Unit.h"
#include "Globals.h"

AIPlayer::AIPlayer(Unit* apParent, std::vector<Unit*>* apEnemies, std::vector<Entity*>* apObjectives)
	: m_pEnemies(apEnemies)
	, m_pObjectives(apObjectives)
	, m_currentCheckpoint(0)
	, m_currentCampingTime(0)
	, m_currentObjectiveState(EObjectiveState::Finding)
	, m_timeTillCheck(s_cMaxTimeTillCheck)
	, m_unitToCheck(0)
	, m_pCurrentTarget(nullptr)
	, m_pSelf(apParent->GetPhysicsModel())
	, m_distanceInterval(s_cDistanceCheckInterval)
	, m_findPathInterval(s_cFindPathInterval)
	, m_pPivot(Pivot::Create())
	, m_findDifferentPathInterval(s_cFindPathInterval)
	, m_previousDistance(9999.f)
{
	m_pParent = apParent;
	m_pSelfHead = m_pParent->GetHead();
}

AIPlayer::~AIPlayer()
{
}

void AIPlayer::OnRespawn()
{
	m_currentObjectiveState = EObjectiveState::Finding;
	m_input.shoot = false;
	m_input.reload = false;
}

Player::EPlayerType AIPlayer::GetType()
{
	return Player::EPlayerType::AI;
}

void AIPlayer::CollectInput()
{
	UpdateWaypoint();
	KillEnemies();
	ManageWeapon();
}

void AIPlayer::UpdateWaypoint()
{
	switch (m_currentObjectiveState)
	{
		case EObjectiveState::Finding:
		{
			m_findPathInterval -= g_deltaTime;

			if (m_findPathInterval <= 0.f)
			{
				m_findPathInterval = s_cFindPathInterval;

				Entity* pWaypoint = (*m_pObjectives)[rand() % m_pObjectives->size()];

				if (g_pWorld->navmesh->FindPath(m_pSelf->GetPosition(true), pWaypoint->GetPosition(true), m_pathToObjective))
				{
					m_currentCheckpoint = 0;
					m_currentObjectiveState = EObjectiveState::MovingTo;
					m_findDifferentPathInterval = s_cFindDifferentPathInterval;
					m_previousDistance = 99999.f;
					m_distanceInterval = s_cDistanceCheckInterval;
				}
				else
				{
					m_input.move = rand() % 2 == 0 ? 1.f : -1.f;
					m_input.strafe = rand() % 2 == 0 ? 1.f : -1.f;
				}
			}
			
			break;
		}

		case EObjectiveState::MovingTo:
		{
			if (m_input.shoot)
			{
				if (!m_input.reload)
				{
					m_input.move = -1.f + float(rand() % 200) * 0.01f;
					m_input.strafe = -1.f + float(rand() % 200) * 0.01f;
				}
				else
				{
					m_input.move = -1.f;
					m_input.strafe = -1.f + float(rand() % 200) * 0.01f;
				}
			}
			else
			{
				m_findDifferentPathInterval -= g_deltaTime;
				if (m_findDifferentPathInterval <= 0.f)
				{
					m_findDifferentPathInterval = s_cFindDifferentPathInterval;
					m_currentObjectiveState = EObjectiveState::Finding;
					break;
				}

				m_distanceInterval -= g_deltaTime;

				if (m_distanceInterval <= 0.f)
				{
					m_distanceInterval = s_cDistanceCheckInterval;

					// Rotation input
					m_pPivot->SetPosition(m_pathToObjective[m_currentCheckpoint], true);
					m_pPivot->Point(m_pSelfHead);

					const auto& rot = m_pPivot->GetRotation(true);

					m_input.rotX = rot.y + 180.f;

					// Update movement input
					m_input.move = 1.f;
					m_input.strafe = 0.f;

					// Did we reach our checkpoint
					float dist = m_pParent->GetPhysicsModel()->GetPosition(true).DistanceToPoint(m_pathToObjective[m_currentCheckpoint]);

					if (dist < m_previousDistance)
					{
						m_previousDistance = dist;
					}
					else
					{
						m_currentCheckpoint++;
						m_previousDistance = 99999.f;
						m_findDifferentPathInterval = s_cFindDifferentPathInterval;

						// Final checkpoint?
						if (m_currentCheckpoint >= m_pathToObjective.size())
						{
							m_currentCampingTime = float(rand() % int32_t(s_cMaxCampingTime * 10.f)) * .1f;
							m_currentObjectiveState = EObjectiveState::Camping;
						}
					}
				}
			}

			break;
		}

		case EObjectiveState::Camping:
		{
			m_currentCampingTime -= g_deltaTime;

			if (m_currentCampingTime <= 0.f)
			{
				m_currentObjectiveState = EObjectiveState::Finding;
				m_currentCheckpoint = 0;
			}
			break;
		}
	}
}

void AIPlayer::KillEnemies()
{
	m_timeTillCheck -= g_deltaTime;

	if (m_timeTillCheck <= 0.f)
	{
		m_timeTillCheck = s_cMaxTimeTillCheck;

		size_t enemyCount = m_pEnemies->size();
		m_input.shoot = false;

		for (size_t i = 0; i < enemyCount; ++i)
		{
			if ((*m_pEnemies)[i]->IsDead())
			{
				continue;
			}

			PickInfo pick;
			Entity* pEnemyHead = (*m_pEnemies)[i]->GetHead();

			m_pParent->GetPhysicsModel()->Hide();
			m_pParent->GetAppearanceModel()->Hide();
			bool hit = g_pWorld->Pick(m_pSelfHead->GetPosition(true), pEnemyHead->GetPosition(true), pick, 0.f, true);
			m_pParent->GetPhysicsModel()->Show();
			m_pParent->GetAppearanceModel()->Show();

			if (hit)
			{
				Entity* pParentEntity = pick.entity;
				while (true)
				{
					if (pParentEntity->parent && pParentEntity->parent->parent)
					{
						pParentEntity = pParentEntity->parent;
					}
					else
					{
						break;
					}
				}

				if (pParentEntity == (*m_pEnemies)[i]->GetAppearanceModel())
				{
					m_pPivot->SetPosition(m_pSelfHead->GetPosition(true), true);
					m_pPivot->Point(pEnemyHead);

					m_input.shoot = true;
					const auto& rot = m_pPivot->GetRotation(true);
					m_input.rotX = rot.y;
					m_input.rotY = -rot.x;

					return;
				}
			}
		}
	}
}

void AIPlayer::ManageWeapon()
{
	if (m_pParent->GetWeapon()->GetCurrentStats().ammo == 0)
	{
		m_input.reload = true;
	}
	else
	{
		m_input.reload = !m_input.shoot;
	}
}
