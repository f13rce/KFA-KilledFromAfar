#pragma once

#include <string>
#include <vector>

#include "Leadwerks.h"
using namespace Leadwerks;

class GameChat
{
public:
	GameChat(Context* apContext);
	~GameChat();

	void AddMessage(const std::string& acMessage);

	void Draw();

private:

	Context* m_pContext;

	struct ChatMessage
	{
		float appearanceTime;
		std::string message;
	};

	static constexpr float s_cChatAppearanceTime = 5.0f;

	std::vector<ChatMessage> m_chatMessages;

};