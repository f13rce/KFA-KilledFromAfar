#include "Player.h"
#include "Unit.h"

Player::Player()
	: m_input({ 0.f, 0.f, 0.f, 0.f, false })
	, m_pParent(nullptr)
{
}

Player::~Player()
{
}

void Player::Update()
{
	CollectInput();

	// Sanitize input
	m_input.move = Math::Clamp(m_input.move, MOVEMENT_MIN, MOVEMENT_MAX);
	m_input.strafe = Math::Clamp(m_input.strafe, MOVEMENT_MIN, MOVEMENT_MAX);
	//m_input.rotX = m_input.rotX % 360; // TODO? Math::Mod()?
	m_input.rotY = Math::Clamp(m_input.rotY, ROTY_MIN, ROTY_MAX);

	if (!m_pParent->IsDead())
	{
		m_pParent->SetInput(m_input);
	}
}

void Player::SetInput(const Input& acInput)
{
	m_input = acInput;
}
