#include "Events/EventSystem.h"

#include <EASTL/optional.h>

#include <mutex>
#include <thread>

using namespace eloo::Events;

EventSystem::EventSystem(size_t initialEventReservationSize) {
    mHashQueueWrite.reserve(initialEventReservationSize);
    mHashQueueRead.reserve(initialEventReservationSize);
    mEventQueueWrite.reserve(initialEventReservationSize);
    mEventQueueRead.reserve(initialEventReservationSize);

    mPollThread = std::thread(&EventSystem::poll, this);
    mNotifyThread = std::thread(&EventSystem::notify, this);
}

EventSystem::~EventSystem() {
    {
        std::unique_lock<std::mutex> lock(mWriteMutex);
        mShutdown = true;
    }

    mThreadWaitForUpdates.notify_all();

    if (mNotifyThread.joinable()) {
        mNotifyThread.join();
    }
    if (mPollThread.joinable()) {
        mPollThread.join();
    }

    // Assert if mNumberOfSubscribers != 0, means some subscribed events
    // were not cleaned up properly
}

void EventSystem::poll() {
    auto waitConditionPredecate = [this]() { return mShutdown || mHasEvents || !mEventQueueWrite.empty(); };

    while (!mShutdown) {
        {
            std::unique_lock<std::mutex> uniqueLock(mWriteMutex);
            mThreadWaitForUpdates.wait(uniqueLock, waitConditionPredecate);
            if (mShutdown) {
                return;
            }

            {
                std::lock_guard<std::mutex> lockGuard(mReadMutex);
                mHashQueueWrite.swap(mHashQueueRead);
                mEventQueueWrite.swap(mEventQueueRead);
            }
            mHasEvents = false;
        }

        mThreadWaitForUpdates.notify_all();
    }
}

void EventSystem::notify() {
    auto waitConditionPredecate = [this]() { return mShutdown || !mEventQueueRead.empty(); };

    while (!mShutdown) {
        eastl::unordered_map<EventHash, EventQueue> batchedEvents;
        {
            std::unique_lock<std::mutex> uniqueLock(mReadMutex);
            mThreadWaitForUpdates.wait(uniqueLock, waitConditionPredecate);
            if (mShutdown) {
                return;
            }

            // Batch events
            for (size_t index = 0; index < mEventQueueRead.size(); ++index) {
                batchedEvents[mHashQueueRead[index]].push_back(eastl::move(mEventQueueRead[index]));
            }
            mHashQueueRead.clear();
            mEventQueueRead.clear();
        }

        // Call the subscribed events
        for (auto& [hash, eventQueue] : batchedEvents) {
            auto& idsAndCallbacks = mSubscribedCallbacks[hash];
            if (eventQueue.empty() || idsAndCallbacks.empty()) {
                continue;
            }

            for (auto& event : eventQueue) {
                for (auto& [_, callback] : idsAndCallbacks) {
                    callback(*event.get());
                }
            }
        }
    }
}