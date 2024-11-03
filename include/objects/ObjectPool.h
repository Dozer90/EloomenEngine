#pragma once

#include "GameObject.h"
#include "Generic/Utilities.h"

#include <EASTL/type_traits.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/utility.h>

namespace eloo
{
/// <summary>
/// Object pool to recycle unused game objects
/// </summary>
template <typename TObject>
class ObjectPool
{
    using ObjectPtr = eastl::unique_ptr<TObject>;

public:
    ObjectPool(size_t maxStorageCount)
        : mMaxStorageCount(maxStorageCount)
    {
        mObjects.reserve(maxStorageCount);
    }

    ObjectPtr acquire()
    {
        if (mObjects.empty())
        {
            return eastl::make_unique<TObject>();
        }
        ObjectPtr poolObject = eastl::move(mObjects.begin());
        mObjects.erase(mObjects.begin());
        return eastl::unique_ptr<TGameObject>(poolObject.release(), ObjectPoolDeleter(this));
    }

private:
    void release(ObjectPtr&& obj)
    {
        if (mObjects.size() < mMaxStorageCount)
        {
            mObjects.push_back(eastl::move(obj));
        }
    }

private:
    const size_t mMaxStorageCount;
    eastl::vector<ObjectPtr> mObjects;
};
}