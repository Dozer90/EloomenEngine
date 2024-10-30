#pragma once

#include "TankBot.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

namespace Scenes
{
	class SceneManager;
}

class Game
{
public:
	Game();

	int run();

	bool registerBot(TankBot&& tankBot);

	static inline Scenes::SceneManager& getSceneManager() { return gSceneManager; }

private:
	static Scenes::SceneManager gSceneManager;

	eastl::vector<eastl::unique_ptr<TankBot>> mTankBots;
};