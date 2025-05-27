#pragma once

#include "window_interface.h"

#include <EASTL/unique_ptr.h>

namespace eloo {
    using window_ptr = eastl::unique_ptr<window_interface>;
    window_ptr create_window(const wchar_t* title, int width, int height);
}