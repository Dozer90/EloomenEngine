#pragma once

#if defined(ELOO_BACKEND_DX12)
#include "backend/dx12/renderer.h"
namespace eloo {
    using renderer = eloo::dx12::renderer;
}
#elif defined(ELOO_BACKEND_VULKAN)
#include "backend/vulkan/renderer.h"
namespace eloo {
    using renderer = eloo::vulkan::renderer;
}
#elif defined(ELOO_BACKEND_METAL)
#include "backend/metal/renderer.h"
namespace eloo {
    using renderer = eloo::metal::renderer;
}
#else
static_assert(false, "No rendering backend defined!");
#endif