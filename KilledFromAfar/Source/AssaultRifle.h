#pragma once

#include "Weapon.h"

class AssaultRifle : public Weapon
{
public:
	AssaultRifle();
	~AssaultRifle();

	virtual void Reset() override;

	virtual std::string GetName() const override;

};
