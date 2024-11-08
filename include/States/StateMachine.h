#pragma once

#include <EASTL/optional.h>
#include <EASTL/functional.h>
#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

namespace eloo
{
using StateID = uint8_t;
constexpr StateID InvalidStateID = static_cast<StateID>(-1);

using StateTransitionID = uint8_t;
constexpr StateTransitionID InvalidStateTransitionID = static_cast<StateTransitionID>(-1);

using StateTransitionCondition = eastl::function<bool()>;
using OnEnterStateCallback = eastl::function<void(StateID)>;
using OnExitStateCallback = eastl::function<void(StateID)>;

class StateMachine
{
private:
	using StateTransitionConditionInternal = eastl::function<StateID()>;

public:
	StateMachine(eastl::string&& name, uint8_t stateCount);

	inline eastl::string_view getName() const { return mStateMachineName; }

	StateID defineState(
		eastl::string&& name,
		eastl::optional<OnEnterStateCallback> onEnter = eastl::nullopt,
		eastl::optional<OnExitStateCallback> onExit = eastl::nullopt);

	StateTransitionID addTransitionCondition(const StateID from, const StateID to, StateTransitionCondition transition);

	void removeTransitionCondition(const StateID from, const StateTransitionID transitionID);

	inline bool setActiveState(StateID newState) { return setState(newState); }

	inline StateID getActiveStateID() const { return mCurrentStateID; }
	inline eastl::string_view getActiveStateName() const { return mNames[mCurrentStateID]; }

	void update(const double deltaTime);

private:
	bool setState(StateID newState);

private:
	eastl::string mStateMachineName;
	StateID mCurrentStateID = 0;
	StateID mNextStateID = 0;

	eastl::vector<eastl::string> mNames;
	eastl::vector<eastl::unique_ptr<eastl::vector<StateTransitionConditionInternal>>> mTransitionConditions;
	eastl::vector<eastl::optional<OnEnterStateCallback>> mOnEnterCallbacks;
	eastl::vector<eastl::optional<OnExitStateCallback>> mOnExitCallbacks;
};
}