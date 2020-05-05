#include "App.h"

App::App()
{
}

App::~App()
{
}

bool App::Start()
{
	std::cout << "Syncing time..." << std::endl;
	Time::Update();
	m_previousTime = std::chrono::steady_clock::now();

	std::cout << "Initializing game..." << std::endl;
	m_game.Init();

	return true;
}

bool App::Loop()
{
	while (true)
	{
		Time::Update();

		auto now = std::chrono::steady_clock::now();
		float delta = float(std::chrono::duration_cast<chrono::microseconds>(now - m_previousTime).count());
		g_deltaTime = delta / 1000000.f;
		m_previousTime = now;

		if (!m_game.Update())
		{
			break;
		}
		m_game.Draw();
	}

	return false;
}
