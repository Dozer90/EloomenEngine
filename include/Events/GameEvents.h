#pragma once

#include "Generic/TypeAliases.h"
#include "Generic/Utilities.h"

#include <Maths/float2.h>

#include <EASTL/unordered_map.h>
#include <EASTL/functional.h>

#include <typeindex>

namespace eloo::Events
{
using EventHash = size_t;
using CallbackID = size_t;

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
    math::float2 location;          // Where the collision detected
    math::float2 overlap;           // How much overlap there was
};

///>=====================================================================

/// <summary>
/// Get the hash value used to index the event
/// </summary>
/// <typeparam name="TEvent">The event to hash</typeparam>
/// <returns>The hashed event type</returns>
template <typename TEvent, ENABLE_TEMPLATE_IF_BASE_OF(IEvent, TEvent)>
EventHash getHash()
{
    return std::type_index(typeid(TEvent)).hash_code();
}

// Defines the initial reserve count for each event-queue's vector in the
// event system. Events that will be called multiple times a frame should
// have a higher reservation size than those that won't
const eastl::unordered_map<EventHash, uint8_t> initialReservationSize =
{
    // Event                          Reservation Size
    { getHash<CollisionEvent>(),             6       },
};

using EventCallback = eastl::function<void(const IEvent&)>;
using EventPtr = eastl::unique_ptr<IEvent>;
}