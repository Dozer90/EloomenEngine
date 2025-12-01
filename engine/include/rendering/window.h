#pragma once

#if defined(ELOO_BACKEND_DX12)
#include "backend/dx12/window.h"
namespace eloo {
    using window = eloo::dx12::window;
}
#elif defined(ELOO_BACKEND_VULKAN)
#include "backend/vulkan/window.h"
namespace eloo {
    using window = eloo::vulkan::window;
}
#elif defined(ELOO_BACKEND_METAL)
#include "backend/metal/window.h"
namespace eloo {
    using window = eloo::metal::window;
}
#else
static_assert(false, "No rendering backend defined!");
#endif