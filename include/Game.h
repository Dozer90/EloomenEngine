#pragma once

#include "Objects/ObjectPool.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

namespace eloo
{
namespace Events { class EventSystem; }
namespace Scenes { class SceneManager; }

class Game
{
public:
	Game();

	int run();

	template <typename TGameObject, ENABLE_TEMPLATE_IF_BASE_OF(GameObject, TGameObject)>
	TGameObject& createGameObject()
	{
		eastl::unique_ptr<TGameObject> obj = mObjectPool.acquire<TGameObject>();
		static_cast<GameObject*>(obj.get())->init();

		mGameObjects.push_back(eastl::move(obj));
		return *(mGameObjects.end() - 1);
	}

	void deleteGameObject(GameObjectID id);

	static inline Events::EventSystem& getEventSystem() { return gEventSystem; }
	static inline Scenes::SceneManager& getSceneManager() { return gSceneManager; }

private:
	static Events::EventSystem gEventSystem;
	static Scenes::SceneManager gSceneManager;

	ObjectPool<GameObject> mObjectPool;
	eastl::vector<GameObjectPtr> mGameObjects;
};
}