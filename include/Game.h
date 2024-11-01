#pragma once

#include "TankBot.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

namespace TankWarz
{
namespace Events
{
class EventSystem;
}
namespace Scenes
{
class SceneManager;
}
class ObjectManager;

class Game
{
public:
	Game();

	int run();

	bool registerBot(TankBot&& tankBot);

	static inline Events::EventSystem& getEventSystem() { return gEventSystem; }
	static inline ObjectManager& getObjectManager() { return gObjectManager; }
	static inline Scenes::SceneManager& getSceneManager() { return gSceneManager; }

private:
	static Events::EventSystem gEventSystem;
	static ObjectManager gObjectManager;
	static Scenes::SceneManager gSceneManager;

	eastl::vector<eastl::unique_ptr<TankBot>> mTankBots;
};
}