#pragma once

#if defined SGE_DEFINE_GAME_EVENT_DEFINITIONS
#if defined SGE_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE
#include SGE_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE
#else
// Test to see if we need to include the data include file
#define SGE_DEFINE_EVENT(x)
#define SGE_DEFINE_EVENT_WITH_DATA(x) \
    static_assert(false, "Event '" #x "' declared as requiring data, but no data include file defined. Use 'SGE_DEFINE_GAME_EVENT_DATA_INCLUDE_FILE'.");
SGE_DEFINE_GAME_EVENT_DEFINITIONS
#undef SGE_DEFINE_EVENT
#undef SGE_DEFINE_EVENT_WITH_DATA
#endif

#include "utility/observer.h"

namespace sge::events::game {
#define SGE_DEFINE_EVENT(x) x,
#define SGE_DEFINE_EVENT_WITH_DATA(x) x,
    enum event_id {
        SGE_DEFINE_GAME_EVENT_DEFINITIONS
        count
    };
#undef SGE_DEFINE_EVENT
#undef SGE_DEFINE_EVENT_WITH_DATA

#define SGE_DEFINE_EVENT(x) inline observer<> on_##x;
#define SGE_DEFINE_EVENT_WITH_DATA(x) inline observer<x##_data> on_##x;
    SGE_DEFINE_GAME_EVENT_DEFINITIONS
#undef SGE_DEFINE_EVENT
#undef SGE_DEFINE_EVENT_WITH_DATA
#undef SGE_DEFINE_GAME_EVENT_DEFINITIONS
}
#endif