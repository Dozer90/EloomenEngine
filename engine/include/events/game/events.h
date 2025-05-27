#pragma once

#if defined ELOO_DEFINE_GAME_EVENT_DEFINITIONS
#if defined ELOO_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE
#include ELOO_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE
#else
// Test to see if we need to include the data include file
#define ELOO_DEFINE_EVENT(x)
#define ELOO_DEFINE_EVENT_WITH_DATA(x) \
    static_assert(false, "Event '" #x "' declared as requiring data, but no data include file defined. Use 'ELOO_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE'.");
ELOO_DEFINE_GAME_EVENT_DEFINITIONS
#undef ELOO_DEFINE_EVENT
#undef ELOO_DEFINE_EVENT_WITH_DATA
#endif

#include "utility/observer.h"

namespace eloo::events::game {
#define ELOO_DEFINE_EVENT(x) x,
#define ELOO_DEFINE_EVENT_WITH_DATA(x) x,
    enum event_id {
        ELOO_DEFINE_GAME_EVENT_DEFINITIONS
        count
    };
#undef ELOO_DEFINE_EVENT
#undef ELOO_DEFINE_EVENT_WITH_DATA

#define ELOO_DEFINE_EVENT(x) observer<> on_##x;
#define ELOO_DEFINE_EVENT_WITH_DATA(x) observer<x##_data> on_##x;
    ELOO_DEFINE_GAME_EVENT_DEFINITIONS
#undef ELOO_DEFINE_EVENT
#undef ELOO_DEFINE_EVENT_WITH_DATA
#undef ELOO_DEFINE_GAME_EVENT_DEFINITIONS
}
#endif