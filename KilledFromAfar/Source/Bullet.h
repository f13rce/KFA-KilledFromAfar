#pragma once

#include "Leadwerks.h"

#include <cstdint>

class Unit;

using namespace Leadwerks;

class Bullet
{
public:
	Bullet();
	~Bullet();

	virtual void Init(Unit* apParent, uint8_t aDamage);

	bool Update();

	Entity* GetModel();

	enum EBulletType
	{
		AssaultRifle,
	};

	EBulletType GetBulletType() const;
	bool IsActive() const;

protected:
	EBulletType m_type;

	Entity* m_pModel;
	Entity* m_pRayOrigin;

	float m_travelSpeed; // units per second
	uint8_t m_damage;
	Unit* m_pParent;

private:
	static constexpr float s_cMaxTravelTime = 3.0f;
	float m_travelTimeLeft;
	bool m_isActive;

};