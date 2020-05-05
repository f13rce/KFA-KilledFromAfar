#include "Bullet.h"
#include "Globals.h"
#include "Game.h"

Bullet::Bullet()
	: m_pModel(Prefab::Load("Prefabs/Bullet.pfb"))
	, m_pRayOrigin(m_pModel->FindChild("Bullet_RayOrigin"))
	, m_travelSpeed(250.f)
	, m_damage(1)
	, m_pParent(nullptr)
	, m_isActive(false)
	, m_travelTimeLeft(s_cMaxTravelTime)
	, m_type(Bullet::EBulletType::AssaultRifle)
{
	m_pModel->Hide();
}

Bullet::~Bullet()
{
}

void Bullet::Init(Unit* apParent, uint8_t aDamage)
{
	m_pParent = apParent;
	m_damage = aDamage;
	m_isActive = true;
	m_travelTimeLeft = s_cMaxTravelTime;
	m_pModel->Show();
}

bool Bullet::Update()
{
	if (!m_isActive)
	{
		return false;
	}

	// Kill the bullet if it has been traveling for long
	m_travelTimeLeft -= g_deltaTime;
	if (m_travelTimeLeft <= 0.f)
	{
		m_isActive = false;
		m_travelTimeLeft = 0.0f;
		m_pModel->Hide();
		return false;
	}

	// Pick locations
	Vec3 curPos;
	Vec3 newPos;
	curPos = m_pRayOrigin->GetPosition(true);
	m_pModel->Move(Vec3(0.f, 0.f, m_travelSpeed * g_deltaTime));
	newPos = m_pRayOrigin->GetPosition(true);
	m_pModel->Hide();

	// Did did it hit something?
	PickInfo pick;	
	if (g_pWorld->Pick(curPos, newPos, pick))
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

		auto pUnit = Game::GetUnitManager()->GetUnit(pParentEntity);

		if (pUnit && pUnit->GetTeam() != m_pParent->GetTeam())
		{
			pUnit->TakeDamage(m_damage);
		}

		Game::GetBulletManager()->SpawnHitParticle(pick.position);

		m_isActive = false;
		return false;
	}

	m_pModel->Show();

	return true;
}

Entity* Bullet::GetModel()
{
	return m_pModel;
}

Bullet::EBulletType Bullet::GetBulletType() const
{
	return m_type;
}

bool Bullet::IsActive() const
{
	return m_isActive;
}
