#pragma once

#include "window_interface.h"
#include "renderer_interface.h"

#include <EASTL/unique_ptr.h>

namespace eloo {
    using renderer_ptr = eastl::unique_ptr<renderer_interface>;
    renderer_ptr create_renderer(window_interface* wnd);
}