#pragma once

#include "ObjectPool.h"

#include <EASTL/unordered_map.h>

namespace TankWarz
{
/// <summary>
/// Central hub that allows the game to register and create new
/// game objects
/// </summary>
class ObjectManager
{
public:
    template <typename TObject>
    bool registerObjectType(size_t maxPoolStorageSize = 16)
    {
        if (ObjectPool<TObject>* pool = getObjectPool<TObject>())
        {
            return false;
        }
        size_t poolMapID = typeid(TObject).hash_code();
        mPools[poolMapID] = eastl::make_unique<ObjectPool<TObject>>(maxPoolStorageSize);
        return true;
    }

    template <typename TObject>
    ObjectPool<TObject>::ObjectPtr createObject()
    {
        if (ObjectPool<TObject>* pool = getObjectPool<TObject>())
        {
            return pool->acquire();
        }
        return ObjectPool<TObject>::ObjectPtr();
    }

    template <typename TObject>
    bool deleteObject(ObjectPool<TObject>::ObjectPtr obj)
    {
        if (ObjectPool<TObject>* pool = getObjectPool<TObject>())
        {
            pool->release(eastl::move(obj));
            return true;
        }
        return false;
    }

private:
    template <typename TObject>
    ObjectPool<TObject>* getObjectPool()
    {
        size_t poolMapID = typeid(TObject).hash_code();
        auto foundPool = mPools.find(poolMapID);
        return foundPool == mPools.end() ? nullptr : static_cast<ObjectPool<TObject>*>(foundPool->second.get());
    }

private:
    eastl::unordered_map<size_t, eastl::unique_ptr<IPool>> mPools;
};
}