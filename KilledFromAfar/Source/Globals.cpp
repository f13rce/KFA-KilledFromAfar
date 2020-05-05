#include "Globals.h"

float g_deltaTime = 0.0f;

World* g_pWorld = nullptr;

Window* g_pWindow = nullptr;
Context* g_pContext = nullptr;

#if USE_LOCAL_PLAYER
Unit* g_pPlayerUnit = nullptr;
#endif