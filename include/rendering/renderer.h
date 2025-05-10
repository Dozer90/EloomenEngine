#pragma once

#if defined ELOO_BACKEND_DX12
#include "dx12/include/rendering/renderer_dx12.h"
#else
#include "core/renderer_interface.h"
#endif

namespace eloo {
    class window_interface;
    using renderer_ptr = eastl::unique_ptr<renderer_interface>;
    renderer_ptr create_renderer(window_interface* wnd);
}