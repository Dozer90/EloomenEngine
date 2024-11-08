#pragma once

#include "GameEvents.h"

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>

#include <condition_variable>

namespace std
{
class mutex;
class thread;
}

namespace eloo::Events
{
class EventSystem
{
private:
    using HashQueue = eastl::vector<EventHash>;
    using EventQueue = eastl::vector<EventPtr>;

public:
    EventSystem(size_t initialEventReservationSize);
    ~EventSystem();

    /// <summary>
    /// Subscribe a static callback to an event
    /// </summary>
    /// <param name="callback">Callback called when the event occurs. Takes a const
    /// event reference as its only parameter</param>
    /// <returns>A CallbackID required when unsubscribing from the event</returns>
    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline CallbackID subscribe(eastl::function<void(const TEvent&)> callback)
    {
        const EventHash hash = Events::getHash<TEvent>();
        std::unique_lock<std::mutex> lock(mWriteMutex);
        if (mSubscribedCallbacks.find(hash) == mSubscribedCallbacks.end())
        {
            mSubscribedCallbacks[hash].reserve(Events::initialReservationSize[hash]);
        }
        mSubscribedCallbacks[hash][mCallbackIDCounter++] = callback;
        mNumberOfSubscribers++;
        return mCallbackIDCounter - 1;
    }

    /// <summary>
    /// Subscribe a callback to an event that will be called for the given target
    /// </summary>
    /// <param name="target">The target to call the function on</param>
    /// <param name="callback">Callback called when the event occurs</param>
    /// <returns>A CallbackID required when unsubscribing from the event</returns>
    template <typename TTarget, typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline CallbackID subscribe(TTarget* target, void(TTarget::*func)(const TEvent&))
    {
        EventCallback callback = [target, func](const TEvent& evt)
        {
            (target->*func)(evt);
        };
        return subscribe(callback);
    }

    /// <summary>
    /// Unsubscribe a callback from an event
    /// </summary>
    /// <param name="id">The CallbackID of the subscribed callback</param>
    /// <returns>True if the successfully unsubscribed from the event</returns>
    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline bool unsubscribe(CallbackID id)
    {
        std::unique_lock<std::mutex> lock(mWriteMutex);
        const EventHash hash = Events::getHash<TEvent>();
        auto found = mSubscribedCallbacks[hash].find(id);
        if (found == mSubscribedCallbacks.end())
        {
            return false;
        }
        mSubscribedCallbacks[hash].erase(found);

        // Check if we can reset the callback counter
        if (--mNumberOfSubscribers == 0)
        {
            mCallbackIDCounter = 0;
        }
        return true;
    }

    /// <summary>
    /// Broadcast an event
    /// </summary>
    /// <param name="evt">The event to broadcast</param>
    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast(TEvent&& evt)
    {
        {
            std::unique_lock<std::mutex> lock(mWriteMutex);
            mHashQueueWrite.push_back(Events::getHash<TEvent>());
            mEventQueueWrite.push_back(eastl::make_unique<TEvent>(eastl::forward<TEvent>(evt)));
            mHasEvents = true;
        }

        mThreadWaitForUpdates.notify_all();
    }

    /// <summary>
    /// Broadcast an event with default data
    /// </summary>
    template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
    inline void broadcast()
    {
        broadcast(TEvent());
    }

private:

    /// <summary>
    /// Poll for any new events and prepare the data for the notify thread
    /// </summary>
    void poll();

    /// <summary>
    /// Notify subscribers when an event occurs
    /// </summary>
    void notify();

private:
    HashQueue mHashQueueWrite, mHashQueueRead;
    EventQueue mEventQueueWrite, mEventQueueRead;

    eastl::unordered_map<EventHash, eastl::unordered_map<CallbackID, EventCallback>> mSubscribedCallbacks;

    // Multi-thread management
    std::mutex mWriteMutex, mReadMutex;
    std::thread mPollThread, mNotifyThread;
    bool mHasEvents = false;
    bool mShutdown = false;
    std::condition_variable mThreadWaitForUpdates;
    size_t mNumberOfSubscribers = 0;
    CallbackID mCallbackIDCounter = 0;
};
}