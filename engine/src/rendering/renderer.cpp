#include "rendering/renderer.h"
#include <stdexcept>

#if defined SGE_BACKEND_DX12
#include "dx12/renderer_dx12.h"
#include "dx12/window_dx12.h"
#else
static_assert(false, "No renderer backend defined");
#endif

sge::renderer_ptr sge::create_renderer(sge::window_interface* wnd) {
#if defined SGE_BACKEND_DX12
    auto* dxWindow = dynamic_cast<window_dx12*>(wnd);
    if (!dxWindow) { throw std::invalid_argument("DX12 requires a DX12 window"); }
    return eastl::make_unique<renderer_dx12>(dxWindow);
#endif
}