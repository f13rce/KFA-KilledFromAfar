#pragma once

#include "Player.h"

class RemotePlayer : public Player
{
public:
	RemotePlayer(Unit* apParent);
	~RemotePlayer();

	virtual void OnRespawn() override;

	virtual void CollectInput() override;

	virtual Player::EPlayerType GetType() override;

};