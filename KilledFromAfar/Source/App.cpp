#include "App.h"
#include "Globals.h"

#include <chrono>

App::App()
	: m_pCamera(nullptr)
	, m_pTexture(Texture::Create(320, 240))
	, m_timeTillRender(0.f)
	, m_currentSecond(0.f)
{
	m_game.SetTestTexture(m_pTexture);
}

App::~App()
{
	m_pCamera->Release();

	g_pWorld->Release();
//#if _DEBUG
	g_pContext->Release();
	g_pWindow->Release();
//#endif
}

bool App::Start()
{
//#if _DEBUG
	std::cout << "Creating window with context..." << std::endl;
//#if USE_LOCAL_PLAYER
	g_pWindow = Window::Create("KFA Server", 10, 10, 1280, 720, Leadwerks::Window::Titlebar | Leadwerks::Window::Resizable);
/*#else
	g_pWindow = Window::Create("KFA Server", 10, 10, 640, 480, Leadwerks::Window::Titlebar | Leadwerks::Window::Resizable);
#endif*/
	g_pContext = Context::Create(g_pWindow);
//#endif

	std::cout << "Creating world..." << std::endl;
	g_pWorld = World::Create();

	std::cout << "Loading map..." << std::endl;
	Map::Load("Maps/SimpleMap.map");
	//Map::Load("Maps/BasicMap.map");

	g_pWorld->SetAmbientLight(1.f, 1.f, 1.f, 1.f);

	std::cout << "Spawning camera..." << std::endl;
	m_pCamera = Camera::Create();
	m_pCamera->SetPosition(Vec3(0.f, 2.f, -30.f));
	m_pCamera->SetClearColor(Vec4(163.f / 255.f, 205.f / 255.f, 224.f / 255.f, 1.f));

	std::cout << "Syncing time..." << std::endl;
	Time::Update();

	std::cout << "Initializing game..." << std::endl;
	m_game.Init();

	return true;
}

bool App::Loop()
{
	while (
//#if _DEBUG
		!g_pWindow->KeyHit(Key::Escape) && !g_pWindow->Closed()
//#else
//		true
//#endif
		)
	{
		Time::Update();
		
		auto start = std::chrono::steady_clock::now();
		m_game.Update();
		auto end = std::chrono::steady_clock::now();
		float gameUpdateTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

		m_timeTillRender -= g_deltaTime;
		if (m_timeTillRender <= 0.f)
		{
			m_timeTillRender += 1.f / s_cTickRate;
			
			start = std::chrono::steady_clock::now();
			m_game.PhysicsUpdate();
			end = std::chrono::steady_clock::now();
			float physicsTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

			start = std::chrono::steady_clock::now();
			g_pWorld->Update();
			end = std::chrono::steady_clock::now();
			float worldUpdateTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

			start = std::chrono::steady_clock::now();
			g_pWorld->Render();
			end = std::chrono::steady_clock::now();
			float worldRenderTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

			start = std::chrono::steady_clock::now();
			m_game.SendPlayerData();
			end = std::chrono::steady_clock::now();
			float sendPlayerDataTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

			start = std::chrono::steady_clock::now();
			m_game.NetworkingUpdate();
			end = std::chrono::steady_clock::now();
			float networkingUpdateTime = float(std::chrono::duration_cast<chrono::milliseconds>(end - start).count());

			float curSec = floorf(float(Time::GetCurrent()) / 1000.f);
			if (curSec > m_currentSecond)
			{
				std::cout << "Tick rate: " << m_deltaTimes.size() << " | Times:" << std::endl;
				std::cout << "\tGame Update: " << gameUpdateTime << "ms" << std::endl;
				std::cout << "\tPhysics: " << physicsTime << "ms" << std::endl;
				std::cout << "\tWorld Update: " << worldUpdateTime << "ms" << std::endl;
				std::cout << "\tWorld Render: " << worldRenderTime << "ms" << std::endl;
				std::cout << "\tSendPlayerData: " << sendPlayerDataTime << "ms" << std::endl;
				std::cout << "\tNetworkingUpdate: " << networkingUpdateTime << "ms" << std::endl;
				m_deltaTimes.clear();
				m_currentSecond = curSec;
			}
			else
			{
				m_deltaTimes.push_back(g_deltaTime);
			}
		}

//#if _DEBUG
		for (size_t i = 0; i < Game::s_networkPlayers.size(); ++i)
		{
			g_pContext->DrawImage(Game::s_networkPlayers[i].GetUnit()->GetRenderTexture(), g_pWindow->GetWidth() / Game::s_networkPlayers.size() * i, 0.f, g_pWindow->GetWidth() / Game::s_networkPlayers.size(), g_pWindow->GetHeight());

			g_pContext->SetBlendMode(Blend::Alpha);
			g_pContext->DrawText(Game::s_networkPlayers[i].GetName(), g_pWindow->GetWidth() / Game::s_networkPlayers.size() * (i+1) - g_pContext->GetFont()->GetTextWidth(Game::s_networkPlayers[i].GetName()), 10);
			g_pContext->DrawText("HP: " + std::to_string(uint32_t(Game::s_networkPlayers[i].GetUnit()->GetHealth())), g_pWindow->GetWidth() / Game::s_networkPlayers.size() * (i+1) - g_pContext->GetFont()->GetTextWidth(Game::s_networkPlayers[i].GetName()), 30);
		}

		//g_pContext->DrawImage(m_pTexture, g_pWindow->GetWidth() / 2, 0.f, g_pWindow->GetWidth() / 2, g_pWindow->GetHeight() / 2);

		g_pContext->SetBlendMode(Blend::Alpha);
		g_pContext->DrawText("Time: " + std::to_string(Time::GetCurrent()), 10, 10);

		g_pContext->DrawText("--- PACKETS ---", 10, 50);
		g_pContext->DrawText("Inc: " + std::to_string(Net13::g_Network->GetIncomingPacketsPerSecond()), 10, 70);
		g_pContext->DrawText("Out: " + std::to_string(Net13::g_Network->GetOutgoingPacketsPerSecond()), 10, 90);
		g_pContext->DrawText("Lst: " + std::to_string(Net13::g_Network->GetPacketsPerSecondLost()), 10, 110);

		g_pContext->DrawText("--- DATA ---", 10, 150);
		g_pContext->DrawText("Inc: " + std::to_string(Net13::g_Network->GetIncomingDataPerSecond()), 10, 170);
		g_pContext->DrawText("Out: " + std::to_string(Net13::g_Network->GetOutgoingDataPerSecond()), 10, 190);

		g_pContext->DrawText("--- MISC ---", 10, 230);
		g_pContext->DrawText("Clients: " + std::to_string(Net13::g_Network->GetActiveConnectionsCount()), 10, 250);

		g_pContext->SetBlendMode(Blend::Solid);
		g_pContext->Sync();
//#endif
	}

	return false;
}
