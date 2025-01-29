#pragma once

#include "Objects/ObjectPool.h"
#include "Events/GameEvents.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

namespace eloo {
class Game {
public:
    template <typename TGameObject, ENABLE_TEMPLATE_IF_BASE_OF(GameObject, TGameObject)>
    TGameObject& createGameObject() {
        eastl::unique_ptr<TGameObject> obj = mObjectPool.acquire<TGameObject>();
        static_cast<GameObject*>(obj.get())->init();

        mGameObjects.push_back(eastl::move(obj));
        return *(mGameObjects.end() - 1);
    }

    void deleteGameObject(GameObjectID id);

    static inline Events::EventSystem& getEventSystem() { return gEventSystem; }

private:
    static Events::EventSystem gEventSystem;

    ObjectPool<GameObject> mObjectPool;
    eastl::vector<GameObjectPtr> mGameObjects;
};
}