#pragma once

#include "GameEvents.h"

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>

#include <mutex>
#include <condition_variable>

namespace eloo::Events
{
class EventSystem
{
public:
    EventSystem(size_t initialEventReservationSize);

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline CallbackID subscribe(eastl::function<void(const TEvent&)> callback)
    {
        const EventHash hash = Events::getHash<TEvent>();
        std::unique_lock<std::mutex> lock(mMutex);
        if (mSubscribedCallbacks.find(hash) == mSubscribedCallbacks.end())
        {
            mSubscribedCallbacks[hash].reserve(Events::initialReservationSize[hash]);
        }
        mSubscribedCallbacks[hash][mCallbackIDCounter++] = callback;
        return mCallbackIDCounter - 1;
    }

    template <typename TTarget, typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline CallbackID subscribe(TTarget* target, void(TTarget::*func)(const TEvent&))
    {
        EventCallback callback = [target, func](const TEvent& evt)
        {
            (target->*func)(evt);
        };
        return subscribe(callback);
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline bool unsubscribe(CallbackID id)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        const EventHash hash = Events::getHash<TEvent>();
        mSubscribedCallbacks[hash].erase(id);
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast(TEvent&& evt)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mHashQueueWrite.push_back(Events::getHash<TEvent>());
        mEventQueueWrite.push_back(eastl::make_unique<TEvent>(eastl::forward<TEvent>(evt)));
        mHasEvents = true;
    }

    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast()
    {
        broadcast(TEvent());
    }

    void poll();

private:
    eastl::vector<EventHash> mHashQueueWrite, mHashQueueRead;
    eastl::vector<EventPtr> mEventQueueWrite, mEventQueueRead;

    eastl::unordered_map<size_t, eastl::unordered_map<CallbackID, EventCallback>> mSubscribedCallbacks;

    // Multi-thread management
    std::mutex mMutex;
    bool mHasEvents = false;
    std::condition_variable mThreadSleepCondition;
    CallbackID mCallbackIDCounter = 0;
};
}