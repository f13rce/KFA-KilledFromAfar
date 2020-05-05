#pragma once

#include "Leadwerks.h"

using namespace Leadwerks;

#include <string>
#include <cstdint>

class Unit;

class Weapon
{
public:
	Weapon();
	~Weapon();

	void Init(Unit* apParent, Entity* apAimingPivot, Entity* apOriginPivot);
	virtual void Reset() = 0;

	void Update();

	void Shoot();
	void Reload();

	virtual std::string GetName() const = 0;
	Entity* GetModel();

	struct WeaponStats
	{
		float firingCooldown; // Seconds
		float reloadTime; // Seconds
		float maxAccuracyPenalty;
		uint8_t ammo;
		uint8_t damage;
	};

	const WeaponStats& GetBaseStats() const;
	const WeaponStats& GetCurrentStats() const;

protected:
	void Reloading();

	Unit* m_pParent;
	Entity* m_pAimingPivot;
	Entity* m_pOriginPivot;

	Entity* m_pModel;
	Entity* m_pBulletSpawn;
	Entity* m_pMuzzleFlash;

	Sound* m_pFiringSound;
	Source* m_pFiringSource;

	Sound* m_pReloadSound;
	Source* m_pReloadSource;

	WeaponStats m_base;
	WeaponStats m_current;

};