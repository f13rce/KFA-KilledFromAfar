#include "RemotePlayer.h"
#include "Unit.h"

RemotePlayer::RemotePlayer(Unit* apParent)
{
	m_pParent = apParent;
	m_pParent->EnableRendering();
}

RemotePlayer::~RemotePlayer()
{
}

void RemotePlayer::OnRespawn()
{
}

void RemotePlayer::CollectInput()
{
}

Player::EPlayerType RemotePlayer::GetType()
{
	return EPlayerType::Remote;
}
