#include "rendering/renderer.h"

#if defined ELOO_BACKEND_DX12
#include "renderer_dx12.h"
#include "window_dx12.h"
#else
static_assert(false, "No renderer backend defined");
#endif

eloo::renderer_ptr eloo::create_renderer(eloo::window_interface* wnd) {
#if defined ELOO_BACKEND_DX12
    return eastl::make_unique<renderer_dx12>(wnd);
#endif
}