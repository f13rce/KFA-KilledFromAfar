#pragma once

#if USE_LOCAL_PLAYER

#include "Player.h"
#include "Leadwerks.h"

using namespace Leadwerks;

class LocalPlayer : public Player
{
public:
	LocalPlayer(Unit* apParent);
	~LocalPlayer();

	virtual void OnRespawn() override;

	virtual void CollectInput() override;

	virtual Player::EPlayerType GetType() override;

protected:

	Vec3 m_previousMousePos;
	float m_mouseSensitivity;

};

#endif