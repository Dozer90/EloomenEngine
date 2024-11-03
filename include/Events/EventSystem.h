#pragma once

#include "GameEvents.h"
#include "Generic/Utilities.h"

#include <EASTL/unique_ptr.h>
#include <EASTL/functional.h>
#include <EASTL/queue.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <typeindex>
#include <mutex>
#include <condition_variable>

namespace eloo::Events
{
class EventSystem
{
private:
    struct QueuedEvent
    {
        size_t hash;
        eastl::unique_ptr<IEvent> event;
    };

public:
    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void subscribe(EventCallback callback)
    {
        const size_t hash = std::type_index(typeid(TEvent)).hash_code();
        std::unique_lock<std::mutex> lock(mMutex);
        mSubscribedCallbacks[hash].push_back(callback);
    }

    template <typename TEvent, typename TTarget, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void subscribe(TTarget* target, void(TTarget::*func)(const TEvent&))
    {
        EventCallback callback = [target, func](const TEvent& evt)
        {
            (target->*func)(evt);
        };
        subscribe<TEvent>(callback);
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline bool unsubscribe(EventCallback callback)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        const size_t hash = std::type_index(typeid(TEvent)).hash_code();
        auto& callbacks = mSubscribedCallbacks[hash];
        auto& entry = eastl::find(callbacks.begin(), callbacks.end(), callback);
        if (entry == callbacks.end())
        {
            return false;
        }
        callbacks.erase(entry);
        return true;
    }

    template <typename TEvent, typename TTarget, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline bool unsubscribe(TTarget* target, void(TTarget::*func)(const TEvent&))
    {
        EventCallback callback = [target, func](const TEvent& evt)
        {
            (target->*func)(evt);
        };
        return unsubscribe<TEvent>(callback);
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast(TEvent&& evt)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mEventQueue.emplace(
            std::type_index(typeid(TEvent)).hash_code(),
            eastl::make_unique<TEvent>(eastl::forward<TEvent>(evt))
        );
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mEventQueue.emplace(
            std::type_index(typeid(TEvent)).hash_code(),
            eastl::make_unique<TEvent>(TEvent())
        );
    }

    void poll();

private:
    eastl::queue<QueuedEvent> mEventQueue;
    eastl::unordered_map<size_t, eastl::vector<EventCallback>> mSubscribedCallbacks;

    // Multi-thread management
    std::mutex mMutex;
    bool mHasEvents = false;
    std::condition_variable mThreadSleepCondition;
};
}