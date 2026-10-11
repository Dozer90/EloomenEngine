#pragma once

#include "events/engine/data.h"

#include "utility/observer.h"

#define SGE_DEFINE_EVENT_DEFINITIONS               \
/* Window events */                                 \
SGE_DEFINE_EVENT_WITH_DATA(window_resized)         \
SGE_DEFINE_EVENT          (window_opened)          \
SGE_DEFINE_EVENT          (window_closed)          \
SGE_DEFINE_EVENT          (window_minimized)       \
SGE_DEFINE_EVENT          (window_maximized)       \
SGE_DEFINE_EVENT_WITH_DATA(window_focus_changed)   \
SGE_DEFINE_EVENT_WITH_DATA(window_moved)           \
/* Input events */                                  \
SGE_DEFINE_EVENT_WITH_DATA(key_pressed)            \
SGE_DEFINE_EVENT_WITH_DATA(key_released)           \
SGE_DEFINE_EVENT_WITH_DATA(mouse_button_pressed)   \
SGE_DEFINE_EVENT_WITH_DATA(mouse_button_released)  \
SGE_DEFINE_EVENT_WITH_DATA(mouse_moved)            \
SGE_DEFINE_EVENT_WITH_DATA(mouse_scrolled)         \
/* Filesystem events */                             \
SGE_DEFINE_EVENT_WITH_DATA(directory_created)      \
SGE_DEFINE_EVENT_WITH_DATA(directory_modified)     \
SGE_DEFINE_EVENT_WITH_DATA(directory_removed)      \
SGE_DEFINE_EVENT_WITH_DATA(file_created)           \
SGE_DEFINE_EVENT_WITH_DATA(file_modified)          \
SGE_DEFINE_EVENT_WITH_DATA(file_removed)           \


namespace sge::events::engine
{
#define SGE_DEFINE_EVENT(x) x,
#define SGE_DEFINE_EVENT_WITH_DATA(x) x,
    enum event_id {
        SGE_DEFINE_EVENT_DEFINITIONS
        count
    };
#undef SGE_DEFINE_EVENT
#undef SGE_DEFINE_EVENT_WITH_DATA

#define SGE_DEFINE_EVENT(x) inline observer<> on_##x;
#define SGE_DEFINE_EVENT_WITH_DATA(x) inline observer<x##_data> on_##x;
    SGE_DEFINE_EVENT_DEFINITIONS
#undef SGE_DEFINE_EVENT
#undef SGE_DEFINE_EVENT_WITH_DATA
#undef SGE_DEFINE_EVENT_DEFINITIONS
}