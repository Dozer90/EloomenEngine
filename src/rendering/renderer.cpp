#include "rendering/renderer.h"

eloo::renderer_ptr eloo::create_renderer(eloo::window_interface* wnd) {
#if defined ELOO_BACKEND_DX12
    return eastl::make_unique<renderer_dx12>(wnd);
#else
    static_assert(false, "No renderer backend defined");
#endif
}