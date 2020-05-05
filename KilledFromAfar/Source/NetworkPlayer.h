#pragma once

#include <cstdint>
#include <string>

class Unit;

class NetworkPlayer
{
public:
	NetworkPlayer(uint16_t aConnectionID, Unit* apUnit, const std::string& acName);
	~NetworkPlayer();

	uint16_t GetConnectionID() const;
	Unit* GetUnit();
	uint8_t GetFrameSequence();
	const std::string& GetName() const;

private:
	uint16_t m_connectionID;
	Unit* m_pUnit;
	std::string m_name;

	uint8_t m_frameSequence;

};