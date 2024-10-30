#pragma once

#include "GameObject.h"

#include <EASTL/type_traits.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/utility.h>

namespace ENGINE_NAMESPACE
{

/// <summary>
/// Generic interface so we can store different types of ObjectPool's in
/// a single unordered_map
/// </summary>
class IPool
{
public:
    inline IPool(size_t maxStorageSize)
        : mMaxStorageSize(maxStorageSize)
    {
    }

protected:
    const size_t mMaxStorageSize;
};

template <typename TObject> class ObjectPool : public IPool;


/// <summary>
/// Custom deleter shall be used to return the acquired object back to the pool
/// when it is released or goes out of scope
/// </summary>
/// <typeparam name="TObject">The object class managed by the pool</typeparam>
template <typename TObject>
class ObjectPoolDeleter
{
public:
    explicit ObjectPoolDeleter(ObjectPool<TObject>* objectPool)
        : mObjectPool(objectPool)
    {}

    void operator()(TObject* obj) const
    {
        obj->onRelease();
        mObjectPool->release(eastl::unique_ptr<TObject>(obj));
    }

private:
    ObjectPool<TObject>* const mObjectPool;
};


/// <summary>
/// Object pool to recycle unused memory
/// </summary>
/// <typeparam name="TObject">The object class this pool manages</typeparam>
template <typename TObject>
class ObjectPool : public IPool
{
    static_assert(eastl::is_base_of<IGameObject, T>::value, "TObject must derive from IGameObject");

public:
    using ObjectPtr = eastl::unique_ptr<TObject, ObjectPoolDeleter<TObject>>;

public:
    ObjectPool(size_t maxStorageSize)
        : IPool(maxStorageSize)
    {
    }

    ObjectPtr acquire()
    {
        if (mObjects.empty())
        {
            return ObjectPtr(new TObject(mIDCounter++), ObjectPoolDeleter<TObject>(this));
        }
        auto poolObject = eastl::move(mObjects.back());
        mObjects.pop_back();
        poolObject->onAcquire(mIDCounter++);
        return ObjectPtr(poolObject.release(), ObjectPoolDeleter<TObject>(this));
    }

    void release(ObjectPtr obj)
    {
        if (mObjects.size() < mMaxStorageSize)
        {
            mObjects.push_back(eastl::move(obj));
        }
    }

    void flush()
    {
        mObjects.clear();
    }

private:
    static GameObjectID mIDCounter = 0;
    eastl::fixed_vector<ObjectPtr> mObjects;
};
}