#include "States/StateMachine.h"

#include "Generic/Utilities.h"

using namespace eloo;

StateMachine::StateMachine(eastl::string&& name, uint8_t stateCount) {
    if (stateCount == InvalidStateID) {
        stateCount--;
    }

    mStateMachineName = eastl::move(name);
    mNames.reserve(stateCount);
    mTransitionConditions.reserve(stateCount);
    mOnEnterCallbacks.reserve(stateCount);
    mOnExitCallbacks.reserve(stateCount);
}

StateID StateMachine::defineState(
    eastl::string&& name,
    eastl::optional<OnEnterStateCallback> onEnter = eastl::nullopt,
    eastl::optional<OnExitStateCallback> onExit = eastl::nullopt) {
    const bool valid = mNextStateID < static_cast<StateID>(mNames.size());
    ASSERT(valid, "Cannot define state %s, already defined %d of %d states.",
           name.c_str(), mNextStateID, mNextStateID);
    if (!valid) {
        return InvalidStateID;
    }

    StateID newStateID = mNextStateID++;
    mNames.push_back(eastl::move(name));
    mTransitionConditions.push_back(eastl::make_unique<eastl::vector<StateTransitionConditionInternal>>());
    mOnEnterCallbacks.push_back(onEnter);
    mOnExitCallbacks.push_back(onExit);
    return newStateID;
}

StateTransitionID StateMachine::addTransitionCondition(const StateID from, const StateID to, StateTransitionCondition transition) {
    eastl::vector<StateTransitionConditionInternal>* conditions = mTransitionConditions[from].get();
    conditions->push_back([=]() { return transition() ? from : to; });
    return static_cast<StateTransitionID>(conditions->size() - 1);
}

void StateMachine::removeTransitionCondition(const StateID from, const StateTransitionID transitionID) {
    eastl::vector<StateTransitionConditionInternal>* conditions = mTransitionConditions[from].get();
    conditions->erase(conditions->begin() + transitionID);
}

void StateMachine::update(const double deltaTime) {
    for (auto&& transitionCondition : *mTransitionConditions[mCurrentStateID].get()) {
        const StateID state = transitionCondition();
        if (state == mCurrentStateID) [[likely]]
        {
            continue;
        }
        setState(state);
        break;
    }
}

bool StateMachine::setState(StateID newState) {
    if (mCurrentStateID == newState) {
        return false;
    }

    eastl::optional<OnExitStateCallback>& exitCallback = mOnExitCallbacks[mCurrentStateID];
    if (exitCallback.has_value()) {
        (*exitCallback)(mCurrentStateID);
    }

    mCurrentStateID = newState;

    eastl::optional<OnEnterStateCallback>& enterCallback = mOnEnterCallbacks[mCurrentStateID];
    if (enterCallback.has_value()) {
        (*enterCallback)(mCurrentStateID);
    }

    return true;
}