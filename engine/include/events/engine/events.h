#pragma once

#include "events/engine/data.h"

#include "utility/observer.h"

#define ELOO_DEFINE_EVENT_DEFINITIONS               \
/* Window events */                                 \
ELOO_DEFINE_EVENT_WITH_DATA(window_resized)         \
ELOO_DEFINE_EVENT          (window_opened)          \
ELOO_DEFINE_EVENT          (window_closed)          \
ELOO_DEFINE_EVENT          (window_minimized)       \
ELOO_DEFINE_EVENT          (window_maximized)       \
ELOO_DEFINE_EVENT_WITH_DATA(window_focus_changed)   \
ELOO_DEFINE_EVENT_WITH_DATA(window_moved)           \
/* Input events */                                  \
ELOO_DEFINE_EVENT_WITH_DATA(key_pressed)            \
ELOO_DEFINE_EVENT_WITH_DATA(key_released)           \
ELOO_DEFINE_EVENT_WITH_DATA(mouse_button_pressed)   \
ELOO_DEFINE_EVENT_WITH_DATA(mouse_button_released)  \
ELOO_DEFINE_EVENT_WITH_DATA(mouse_moved)            \
ELOO_DEFINE_EVENT_WITH_DATA(mouse_scrolled)         \
/* Filesystem events */                             \
ELOO_DEFINE_EVENT_WITH_DATA(directory_created)      \
ELOO_DEFINE_EVENT_WITH_DATA(directory_modified)     \
ELOO_DEFINE_EVENT_WITH_DATA(directory_removed)      \
ELOO_DEFINE_EVENT_WITH_DATA(file_created)           \
ELOO_DEFINE_EVENT_WITH_DATA(file_modified)          \
ELOO_DEFINE_EVENT_WITH_DATA(file_removed)           \


namespace eloo::events::engine
{
#define ELOO_DEFINE_EVENT(x) x,
#define ELOO_DEFINE_EVENT_WITH_DATA(x) x,
    enum event_id {
        ELOO_DEFINE_EVENT_DEFINITIONS
        count
    };
#undef ELOO_DEFINE_EVENT
#undef ELOO_DEFINE_EVENT_WITH_DATA

#define ELOO_DEFINE_EVENT(x) observer<> on_##x;
#define ELOO_DEFINE_EVENT_WITH_DATA(x) observer<x##_data> on_##x;
    ELOO_DEFINE_EVENT_DEFINITIONS
#undef ELOO_DEFINE_EVENT
#undef ELOO_DEFINE_EVENT_WITH_DATA
#undef ELOO_DEFINE_EVENT_DEFINITIONS
}