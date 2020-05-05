#include "NetworkPlayer.h"

NetworkPlayer::NetworkPlayer(uint16_t aConnectionID, Unit* apUnit, const std::string& acName)
	: m_connectionID(aConnectionID)
	, m_pUnit(apUnit)
	, m_name(acName)
	, m_frameSequence(0)
{
}

NetworkPlayer::~NetworkPlayer()
{
}

uint16_t NetworkPlayer::GetConnectionID() const
{
	return m_connectionID;
}

Unit* NetworkPlayer::GetUnit()
{
	return m_pUnit;
}

uint8_t NetworkPlayer::GetFrameSequence()
{
	m_frameSequence++;
	return m_frameSequence;
}

const std::string& NetworkPlayer::GetName() const
{
	return m_name;
}
