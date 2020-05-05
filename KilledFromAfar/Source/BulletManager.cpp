#include "BulletManager.h"
#include "Globals.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::Init()
{
}

void BulletManager::Update()
{
	UpdateBullets();
	UpdateHitParticles();
}

Bullet* BulletManager::Spawn(Bullet::EBulletType aType)
{
	auto pBullet = GetDisabledBulletOfType(aType);
	if (pBullet)
	{
		m_enabledBullets.push_back(pBullet);
		return pBullet;
	}
	else
	{
		std::unique_ptr<Bullet> pBullet;

		switch (aType)
		{
		case Bullet::EBulletType::AssaultRifle:
		{
			pBullet = std::make_unique<Bullet>();
			break;
		}
		}

		Bullet* pBulletRawPtr = nullptr;
		if (pBullet)
		{
			m_bullets.push_back(std::move(pBullet));
			pBulletRawPtr = m_bullets.back().get();
			m_enabledBullets.push_back(pBulletRawPtr);
		}

		return pBulletRawPtr;
	}
}

void BulletManager::SpawnHitParticle(const Vec3& acImpactPosition)
{
	Entity* pHitParticle = GetDisabledHitParticle();

	if (!pHitParticle)
	{
		pHitParticle = Prefab::Load("Prefabs/HitParticle.pfb");
	}

	pHitParticle->SetScale(1.f);
	pHitParticle->Show();
	pHitParticle->SetPosition(acImpactPosition, true);
	m_enabledHitParticles.push_back(pHitParticle);
}

void BulletManager::UpdateBullets()
{
	if (m_enabledBullets.empty())
	{
		return;
	}

	for (int32_t i = m_enabledBullets.size() - 1; i >= 0; --i)
	{
		if (!m_enabledBullets[i]->Update())
		{
			m_disabledBullets.push_back(m_enabledBullets[i]);
			m_enabledBullets.erase(m_enabledBullets.begin() + i);
		}
	}
}

void BulletManager::UpdateHitParticles()
{
	if (m_enabledHitParticles.empty())
	{
		return;
	}

	for (int32_t i = m_enabledHitParticles.size() - 1; i >= 0; --i)
	{
		float size = m_enabledHitParticles[i]->GetScale(false).x;
		size -= g_deltaTime;

		if (size <= 0.f)
		{
			m_enabledHitParticles[i]->Hide();
			m_disabledHitParticles.push_back(m_enabledHitParticles[i]);
			m_enabledHitParticles.erase(m_enabledHitParticles.begin() + i);
		}
		else
		{
			m_enabledHitParticles[i]->SetScale(size);
		}
	}
}

Bullet* BulletManager::GetDisabledBulletOfType(Bullet::EBulletType aType)
{
	for (size_t i = 0; i < m_disabledBullets.size(); ++i)
	{
		if (m_disabledBullets[i]->GetBulletType() == Bullet::EBulletType::AssaultRifle)
		{
			auto pBullet = m_disabledBullets[i];
			m_disabledBullets.erase(m_disabledBullets.begin() + i);

			return pBullet;
		}
	}

	return nullptr;
}

Entity* BulletManager::GetDisabledHitParticle()
{
	if (m_disabledHitParticles.empty())
	{
		return nullptr;
	}

	Entity* pEntity = m_disabledHitParticles.back();
	m_disabledHitParticles.pop_back();
	return pEntity;
}
