#if USE_LOCAL_PLAYER

#include "LocalPlayer.h"
#include "Globals.h"

LocalPlayer::LocalPlayer(Unit* apParent)
{
	m_pParent = apParent;
	m_pParent->EnableRendering();

	g_pPlayerUnit = apParent;

	g_pWindow->SetMousePosition(g_pWindow->GetWidth() * .5f, g_pWindow->GetHeight() * .5f);
	m_previousMousePos = g_pWindow->GetMousePosition();

	m_mouseSensitivity = 0.1f;
}

LocalPlayer::~LocalPlayer()
{
}

void LocalPlayer::OnRespawn()
{
}

void LocalPlayer::CollectInput()
{
	m_input.move = (g_pWindow->KeyDown(Key::W) ? 1.f : 0.f) - (g_pWindow->KeyDown(Key::S) ? 1.f : 0.f);
	m_input.strafe = (g_pWindow->KeyDown(Key::D) ? 1.f : 0.f) - (g_pWindow->KeyDown(Key::A) ? 1.f : 0.f);
	m_input.crouch = g_pWindow->KeyDown(Key::ControlKey);
	m_input.shoot = g_pWindow->MouseDown(1);
	m_input.reload = g_pWindow->KeyDown(Key::R);
	m_input.aimDownSight = g_pWindow->MouseDown(2);

	Vec3 mousePos = g_pWindow->GetMousePosition();

	Vec3 delta = m_previousMousePos - mousePos;
	m_input.rotX -= delta.x * m_mouseSensitivity;
	m_input.rotY += delta.y * m_mouseSensitivity;

	g_pWindow->SetMousePosition(g_pWindow->GetWidth() * .5f, g_pWindow->GetHeight() * .5f);
	m_previousMousePos = g_pWindow->GetMousePosition();
}

Player::EPlayerType LocalPlayer::GetType()
{
	return Player::EPlayerType::Local;
}

#endif