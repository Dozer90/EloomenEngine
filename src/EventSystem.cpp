#include "Events/EventSystem.h"

#include <EASTL/optional.h>

using namespace eloo::Events;

EventSystem::EventSystem(size_t initialEventReservationSize)
{
    mHashQueueWrite.reserve(initialEventReservationSize);
    mHashQueueRead.reserve(initialEventReservationSize);
    mEventQueueWrite.reserve(initialEventReservationSize);
    mEventQueueRead.reserve(initialEventReservationSize);
}

void EventSystem::poll()
{
    auto waitConditionPredecate = [this]() { return mHasEvents || !mEventQueueWrite.empty(); };

    eastl::unordered_map<EventHash, EventPtr> batchedEvents;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mThreadSleepCondition.wait(lock, waitConditionPredecate);
            mHashQueueWrite.swap(mHashQueueRead);
            mEventQueueWrite.swap(mEventQueueRead);
            mHasEvents = false;
        }

        for (size_t index = 0; index < mEventQueueRead.size(); ++index)
        {
            batchedEvents[mHashQueueRead[index]].push_back(eastl::move(mEventQueueRead[index]));
        }
        mHashQueueRead.clear();
        mEventQueueRead.clear();

        for (auto& [hash, events] : batchedEvents)
        {
            auto& callbacks = mSubscribedCallbacks[hash];
            for (auto& event : events)
            {
                for (auto& [_, callback] : callbacks)
                {
                    callback(*event.get());
                }
            }
        }
    }
}