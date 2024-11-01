#include "Events/EventSystem.h"

#include <EASTL/optional.h>

namespace TankWarz::Events
{

void EventSystem::poll()
{
    auto waitConditionPredecate = [this]() { return mHasEvents || !mEventQueue.empty(); };
    while (true)
    {
        eastl::optional<QueuedEvent> queuedEvent = eastl::nullopt;
        {
            std::unique_lock<std::mutex> lock(mMutex);

            mThreadSleepCondition.wait(lock, waitConditionPredecate);
            if (mEventQueue.empty())
            {
                continue;
            }

            queuedEvent = eastl::move(mEventQueue.front());
            mEventQueue.pop();
            mHasEvents = !mEventQueue.empty();
        }

        if (!queuedEvent.has_value())
        {
            continue;
        }

        auto& callbacks = mSubscribedCallbacks[queuedEvent->hash];
        for (auto& callback : callbacks)
        {
            callback(*queuedEvent->event.get());
        }
    }
}

}