#pragma once

#include "Generic/TypeAliases.h"

#include <SFML/System/Vector2.hpp>
#include <EASTL/functional.h>

namespace eloo::Events
{
///>=====================================================================
/// Base event
struct IEvent
{
    virtual ~IEvent() = default;
};

///>=====================================================================
/// Collision event
struct CollisionEvent : public IEvent
{
    GameObjectID firstID;           // First object involved
    GameObjectID secondID;          // Second object involved
    sf::Vector2<double> location;   // Where the collision detected
    sf::Vector2<double> overlap;    // How much overlap there was
};

///>=====================================================================

using EventCallback = eastl::function<void(const IEvent&)>;
}