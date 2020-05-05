#include "Weapon.h"
#include "Globals.h"
#include "Game.h"

Weapon::Weapon()
	: m_pModel(nullptr)
	, m_pFiringSound(nullptr)
	, m_pFiringSource(nullptr)
	, m_pReloadSound(nullptr)
	, m_pReloadSource(nullptr)
	, m_pParent(nullptr)
	, m_pBulletSpawn(nullptr)
	, m_pMuzzleFlash(nullptr)
	, m_pAimingPivot(nullptr)
	, m_pOriginPivot(nullptr)
{
}

Weapon::~Weapon()
{
	if (m_pFiringSource)
	{
		m_pFiringSource->Release();
		m_pFiringSource = nullptr;
	}

	if (m_pReloadSource)
	{
		m_pReloadSource->Release();
		m_pReloadSource = nullptr;
	}
}

void Weapon::Init(Unit* apParent, Entity* apAimingPivot, Entity* apOriginPivot)
{
	m_pParent = apParent;
	m_pAimingPivot = apAimingPivot;
	m_pOriginPivot = apOriginPivot;
}

void Weapon::Update()
{
	if (m_current.firingCooldown > 0.f)
	{
		m_current.firingCooldown -= g_deltaTime;
		if (m_current.firingCooldown < 0.f)
		{
			m_current.firingCooldown = 0.f;
		}
	}

	Reloading();

	if (!m_pMuzzleFlash->Hidden())
	{
		m_pMuzzleFlash->Hide();
	}
}

void Weapon::Shoot()
{
	if (m_current.firingCooldown > 0.f || m_current.reloadTime > 0.f || m_current.ammo == 0)
	{
		return;
	}

	m_current.firingCooldown = m_base.firingCooldown;
	m_current.ammo--;

	auto pBulletManager = Game::GetBulletManager();
	auto pBullet = pBulletManager->Spawn(Bullet::EBulletType::AssaultRifle);
	pBullet->Init(m_pParent, m_current.damage);
	pBullet->GetModel()->SetPosition(m_pBulletSpawn->GetPosition(true), true);

	PickInfo pick;
	m_pAimingPivot->SetPosition(Vec3(0.f, 0.f, 10000.f), false);
	m_pOriginPivot->GetParent()->Hide();
	if (g_pWorld->Pick(m_pOriginPivot->GetPosition(true), m_pAimingPivot->GetPosition(true), pick))
	{
		m_pAimingPivot->SetPosition(pick.position, true);
	}
	m_pOriginPivot->GetParent()->Show();

	m_pAimingPivot->SetPosition(
		Vec3(
			-(m_base.maxAccuracyPenalty * .5f) + float(rand() % uint32_t(m_base.maxAccuracyPenalty * 100.f)) / 100.f,
			-(m_base.maxAccuracyPenalty * .5f) + float(rand() % uint32_t(m_base.maxAccuracyPenalty * 100.f)) / 100.f,
			m_pAimingPivot->GetPosition(false).z
		), false);

	pBullet->GetModel()->Point(m_pAimingPivot);

	m_pMuzzleFlash->Show();

#if USE_LOCAL_PLAYER
	m_pFiringSource->SetPosition(m_pModel->GetPosition(true));
	m_pFiringSource->Play();
#endif
}

void Weapon::Reload()
{
	if (m_current.reloadTime > 0.f || m_current.ammo == m_base.ammo)
	{
		return;
	}

	m_current.reloadTime = m_base.reloadTime;

#if USE_LOCAL_PLAYER
	m_pReloadSource->SetPosition(m_pModel->GetPosition(true));
	m_pReloadSource->Play();
#endif
}

Entity* Weapon::GetModel()
{
	return m_pModel;
}

const Weapon::WeaponStats& Weapon::GetBaseStats() const
{
	return m_base;
}

const Weapon::WeaponStats& Weapon::GetCurrentStats() const
{
	return m_current;
}

void Weapon::Reloading()
{
	if (m_current.reloadTime > 0.f)
	{
#if USE_LOCAL_PLAYER
		m_pReloadSource->SetPosition(m_pModel->GetPosition(true));
#endif
		m_current.reloadTime -= g_deltaTime;

		if (m_current.reloadTime <= 0.f)
		{
			m_current.ammo = m_base.ammo;
			m_current.reloadTime = 0.f;
		}
	}
}
