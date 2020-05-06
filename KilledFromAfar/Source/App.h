#pragma once

#undef GetFileType

#include "Game.h"

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

	Material* m_pMaterial;
	Texture* m_pTexture;

	static constexpr float s_cTickRate = 60.0f;
	float m_timeTillRender;

	std::list<float> m_deltaTimes;
	float m_currentSecond;

};
