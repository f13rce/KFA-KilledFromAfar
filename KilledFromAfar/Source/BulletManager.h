#pragma once

#include <vector>

#include "Bullet.h"

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void Init();

	void Update();

	Bullet* Spawn(Bullet::EBulletType aType);
	void SpawnHitParticle(const Vec3& acImpactPosition);

protected:
	void UpdateBullets();
	void UpdateHitParticles();

	Bullet* GetDisabledBulletOfType(Bullet::EBulletType aType);
	Entity* GetDisabledHitParticle();

	std::vector<std::unique_ptr<Bullet>> m_bullets;
	std::vector<Bullet*> m_enabledBullets;
	std::vector<Bullet*> m_disabledBullets;

	std::vector<Entity*> m_enabledHitParticles;
	std::vector<Entity*> m_disabledHitParticles;

};