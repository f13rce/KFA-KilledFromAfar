#include "GameChat.h"

GameChat::GameChat(Context* apContext)
	: m_pContext(apContext)
{
}

GameChat::~GameChat()
{
}

void GameChat::AddMessage(const std::string& acMessage)
{
	ChatMessage msg;
	msg.appearanceTime = s_cChatAppearanceTime;
	msg.message = acMessage;

	m_chatMessages.push_back(msg);
}

void GameChat::Draw()
{
}
