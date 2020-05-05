#pragma once

#include "Leadwerks.h"
#include <thread>
#include <mutex>

using namespace Leadwerks;

extern float g_deltaTime;

extern World* g_pWorld;

#define NAME_KEY "name"

extern Window* g_pWindow;
extern Context* g_pContext;

#if USE_LOCAL_PLAYER
#include "Unit.h"
extern Unit* g_pPlayerUnit;
#endif
