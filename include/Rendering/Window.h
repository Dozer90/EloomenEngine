#pragma once

#if defined ELOO_BACKEND_DX12
#include "dx12/include/rendering/window_dx12.h"
#else
#include "core/window_interface.h"
#endif

namespace eloo {
    using window_ptr = eastl::unique_ptr<window_interface>;
    window_ptr create_window(const eastl::wstring& title, int width, int height);
}