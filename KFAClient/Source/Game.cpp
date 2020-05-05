#include "Game.h"

#include "data/messages/NetcodeMessages.h"

Game* g_pGame = nullptr;
float g_deltaTime = 0.f;

Game::Game()
	: m_pWindow(nullptr)
	, m_pContext(nullptr)
	, m_pFrame(nullptr)
	, m_networkManager(13338, false)
	, m_connectCooldown(0.f)
	, m_gameChat(nullptr)
	, m_frameSequence(0)
	, m_input(std::make_shared<Net13::Messages::PlayerInput>())
	, m_inputTick(1.f / 60.f)
	, m_joinedGame(false)
	, m_sentJoinGameRequest(false)
{
	m_pWindow = Window::Create("KFA - It's Valoran't, free of any anti-cheat and hack-free", 10, 10, 1280, 720, Window::Titlebar | Window::Resizable);
	m_pContext = Context::Create(m_pWindow);

	m_pFrame = Texture::Create(320, 240);

	m_gameChat = GameChat(m_pContext);

	RegisterCustomNetworkMessages();
	m_networkManager.SetTickRate(144);

	m_input->aimDownSight = false;
	m_input->crouch = false;
	m_input->move = 0.f;
	m_input->strafe = 0.f;
	m_input->reload = false;
	m_input->shoot = false;
	m_input->rotX = 0.f;
	m_input->rotY = 0.f;

	g_pGame = this;
}

Game::~Game()
{
	m_networkManager.Disconnect();
}

void Game::Init()
{
	std::cout << "Finished initializing game!" << std::endl;
}

void Game::Connect()
{
	if (m_connectCooldown > 0.f)
	{
		m_connectCooldown -= g_deltaTime;
	}
	else
	{
		std::cout << "Connecting to server..." << std::endl;
		m_networkManager.ConnectTo("127.0.0.1", s_cServerPort);
		m_connectCooldown = 15.f;
	}
}

bool Game::Update()
{
	if (!m_networkManager.IsConnected())
	{
		Connect();
	}
	else
	{
		if (m_joinedGame)
		{
			UpdateInput();
			m_inputTick -= g_deltaTime;
			if (m_inputTick <= 0.f)
			{
				m_inputTick = 1.f / 60.f;
				m_networkManager.Send(m_input);
			}
		}
		else
		{
			if (!m_sentJoinGameRequest)
			{
				std::cout << "Sending join game request..." << std::endl;
				auto pMessage = std::make_shared<Net13::Messages::JoinGameRequest>();
				pMessage->name = "f13rce";
				m_networkManager.SendReliable(pMessage);
				m_sentJoinGameRequest = true;
			}
		}
	}

	m_networkManager.Update();

	return !m_pWindow->KeyDown(Key::Escape);
}

void Game::Draw()
{
	m_pContext->Clear();
	m_pContext->DrawImage(m_pFrame, 0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());

	m_pContext->SetBlendMode(Blend::Alpha);
	m_pContext->DrawText("Current frame sequence: " + std::to_string(uint32_t(m_frameSequence)), 10, 10);

	m_pContext->DrawText("--- PACKETS ---" + std::to_string(Time::GetCurrent()), 10, 50);
	m_pContext->DrawText("Inc: " + std::to_string(Net13::g_Network->GetIncomingPacketsPerSecond()), 10, 70);
	m_pContext->DrawText("Out: " + std::to_string(Net13::g_Network->GetOutgoingPacketsPerSecond()), 10, 90);
	m_pContext->DrawText("Lst: " + std::to_string(Net13::g_Network->GetPacketsPerSecondLost()), 10, 110);

	m_pContext->DrawText("--- DATA ---" + std::to_string(Time::GetCurrent()), 10, 150);
	m_pContext->DrawText("Inc: " + std::to_string(Net13::g_Network->GetIncomingDataPerSecond()), 10, 170);
	m_pContext->DrawText("Out: " + std::to_string(Net13::g_Network->GetOutgoingDataPerSecond()), 10, 190);

	m_pContext->DrawText("--- MISC ---" + std::to_string(Time::GetCurrent()), 10, 230);
	m_pContext->DrawText("Ping: " + std::to_string(Net13::g_Network->GetAveragePing()), 10, 250);

	m_pContext->SetBlendMode(Blend::Solid);

	m_pContext->Sync(false);
}

GameChat* Game::GetGameChat()
{
	return &m_gameChat;
}

void Game::OnJoinSuccessful()
{
	if (m_joinedGame)
	{
		return;
	}

	std::cout << "We're in!" << std::endl;

	m_sentJoinGameRequest = false;
	m_joinedGame = true;
}

void Game::OnJoinUnsuccessful()
{
	m_sentJoinGameRequest = false;
	m_joinedGame = false;
}

void Game::OnPixelColorIDReceived(const Net13::Messages::PixelColorID& acMessage)
{
	m_compressor.RegisterPixelID(acMessage.id, acMessage.r, acMessage.g, acMessage.b);
}

void Game::OnFrameReceived(uint8_t aSequence, const std::string& acFrameBuffer, uint32_t aOriginalProtocolSize)
{
	if (SequenceIsNewer(aSequence))
	{
		m_frameSequence = aSequence;

		m_compressor.Decompress(m_pFrame, acFrameBuffer, aOriginalProtocolSize);
	}
}

void Game::RegisterCustomNetworkMessages()
{
	auto pJoinGameRequest = std::make_unique<Net13::Messages::JoinGameRequest>();
	m_networkManager.RegisterMessage(std::move(pJoinGameRequest), pJoinGameRequest->GetMessageOpcode());

	auto pJoinGameResponse = std::make_unique<Net13::Messages::JoinGameResponse>();
	m_networkManager.RegisterMessage(std::move(pJoinGameResponse), pJoinGameResponse->GetMessageOpcode());

	auto pChatMessage = std::make_unique<Net13::Messages::ChatMessage>();
	m_networkManager.RegisterMessage(std::move(pChatMessage), pChatMessage->GetMessageOpcode());

	auto pSystemMessage = std::make_unique<Net13::Messages::SystemMessage>();
	m_networkManager.RegisterMessage(std::move(pSystemMessage), pSystemMessage->GetMessageOpcode());

	auto pPlay2DSound = std::make_unique<Net13::Messages::Play2DSound>();
	m_networkManager.RegisterMessage(std::move(pPlay2DSound), pPlay2DSound->GetMessageOpcode());

	auto pPlay3DSound = std::make_unique<Net13::Messages::Play3DSound>();
	m_networkManager.RegisterMessage(std::move(pPlay3DSound), pPlay3DSound->GetMessageOpcode());

	auto pPlayerInput = std::make_unique<Net13::Messages::PlayerInput>();
	m_networkManager.RegisterMessage(std::move(pPlayerInput), pPlayerInput->GetMessageOpcode());

	auto pFrame = std::make_unique<Net13::Messages::Frame>();
	m_networkManager.RegisterMessage(std::move(pFrame), pFrame->GetMessageOpcode());

	auto pPixelColorID = std::make_unique<Net13::Messages::PixelColorID>();
	m_networkManager.RegisterMessage(std::move(pPixelColorID), pPixelColorID->GetMessageOpcode());
}

void Game::UpdateInput()
{
	m_input->aimDownSight = m_pWindow->MouseDown(2);
	m_input->crouch = m_pWindow->KeyDown(Key::ControlKey);
	m_input->move = (m_pWindow->KeyDown(Key::W) ? 1.f : 0.f) - (m_pWindow->KeyDown(Key::S) ? 1.f : 0.f);
	m_input->strafe = (m_pWindow->KeyDown(Key::D) ? 1.f : 0.f) - (m_pWindow->KeyDown(Key::A) ? 1.f : 0.f);
	m_input->reload = m_pWindow->KeyDown(Key::R);
	m_input->shoot = m_pWindow->MouseDown(1);

	Vec3 mousePos = m_pWindow->GetMousePosition();

	float mouseSensitivity = 0.1f;

	m_input->rotX += (mousePos.x - m_pWindow->GetWidth() * .5f) * mouseSensitivity;
	m_input->rotY -= (mousePos.y - m_pWindow->GetHeight() * .5f) * mouseSensitivity;

	// Sanitize
	m_input->move = Math::Clamp(m_input->move, MOVEMENT_MIN, MOVEMENT_MAX);
	m_input->strafe = Math::Clamp(m_input->strafe, MOVEMENT_MIN, MOVEMENT_MAX);
	//m_input.rotX = m_input.rotX % 360; // TODO? Math::Mod()?
	m_input->rotY = Math::Clamp(m_input->rotY, ROTY_MIN, ROTY_MAX);

	m_pWindow->SetMousePosition(m_pWindow->GetWidth() * .5f, m_pWindow->GetHeight() * .5f);
}

bool Game::SequenceIsNewer(uint8_t aSequence)
{
	if (aSequence > m_frameSequence)
	{
		return true;
	}

	if (aSequence < m_frameSequence && (m_frameSequence - aSequence >= UINT8_MAX / 2))
	{
		return true;
	}

	return false;
}

void Net13::Messages::JoinGameRequest_Receive(const Net13::Messages::JoinGameRequest& acMessage)
{
}

void Net13::Messages::JoinGameResponse_Receive(const Net13::Messages::JoinGameResponse& acMessage)
{
	switch (acMessage.response)
	{
		case uint8_t(0):
		{
			std::cout << "Server gave no response... Retrying again later." << std::endl;
			g_pGame->OnJoinUnsuccessful();
			break;
		}

		case uint8_t(1):
		{
			g_pGame->OnJoinSuccessful();
			break;
		}

		case uint8_t(2):
		{
			std::cout << "Server is full... Retrying again later." << std::endl;
			g_pGame->OnJoinUnsuccessful();
			break;
		}
	}
}

void Net13::Messages::ChatMessage_Receive(const Net13::Messages::ChatMessage& acMessage)
{
	g_pGame->GetGameChat()->AddMessage(acMessage.message);
}

void Net13::Messages::SystemMessage_Receive(const Net13::Messages::SystemMessage& acMessage)
{
}

void Net13::Messages::Play2DSound_Receive(const Net13::Messages::Play2DSound& acMessage)
{
}

void Net13::Messages::Play3DSound_Receive(const Net13::Messages::Play3DSound& acMessage)
{
}

void Net13::Messages::PlayerInput_Receive(const Net13::Messages::PlayerInput& acMessage)
{
}

void Net13::Messages::Frame_Receive(const Net13::Messages::Frame& acMessage)
{
	g_pGame->OnFrameReceived(acMessage.sequence, acMessage.buffer, acMessage.originalSize);
}

void Net13::Messages::PixelColorID_Receive(const Net13::Messages::PixelColorID& acMessage)
{
	g_pGame->OnPixelColorIDReceived(acMessage);
}
