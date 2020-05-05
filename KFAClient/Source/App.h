#pragma once

#undef GetFileType

#include "Game.h"
#include <chrono>

class App
{
public:
	App();
	virtual ~App();

    bool Start();
    bool Loop();

private:
	Game m_game;
	Camera* m_pCamera;

	std::chrono::time_point<std::chrono::steady_clock> m_previousTime;

};
