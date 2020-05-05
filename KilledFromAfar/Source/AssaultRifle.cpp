#include "AssaultRifle.h"
#include "Globals.h"

AssaultRifle::AssaultRifle()
{
	m_base.ammo = 48;
	m_base.damage = 15;
	m_base.firingCooldown = 1.f / 16.f;
	m_base.reloadTime = 2.75f;
	m_base.maxAccuracyPenalty = 1.75f;

	m_pModel = Prefab::Load("Prefabs/AssaultRifle.pfb");
	m_pBulletSpawn = m_pModel->FindChild("BulletSpawn");
	m_pMuzzleFlash = m_pModel->FindChild("MuzzleFlash");
	

#if USE_LOCAL_PLAYER
	m_pFiringSound = Sound::Load("Sound/AR_Shot.wav");
	m_pReloadSound = Sound::Load("Sound/AR_Reload.wav");

	m_pFiringSource = Source::Create();
	m_pReloadSource = Source::Create();

	m_pFiringSource->SetSound(Sound::Load("Sound/AR_Shot_Mono.wav"));
	m_pFiringSource->SetLoopMode(false);
	m_pFiringSource->SetRange(250.f);

	m_pReloadSource->SetSound(Sound::Load("Sound/AR_Reload_Mono.wav"));
	m_pReloadSource->SetLoopMode(false);
	m_pReloadSource->SetRange(250.f);
#endif

	Reset();
}

AssaultRifle::~AssaultRifle()
{
}

void AssaultRifle::Reset()
{
	m_current = m_base;
	m_current.firingCooldown = 0.f;
	m_current.reloadTime = 0.f;

	m_pMuzzleFlash->Hide();
}

std::string AssaultRifle::GetName() const
{
	return "M4K7";
}

