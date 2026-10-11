#include "rendering/window.h"

#if defined SGE_BACKEND_DX12
#include "dx12/renderer_dx12.h"
#include "dx12/window_dx12.h"
#else
static_assert(false, "No renderer backend defined");
#endif

#include "events/engine/events.h"

#include <Windows.h>


sge::window_ptr sge::create_window(const wchar_t* title, int width, int height) {
#if defined SGE_BACKEND_DX12
    return eastl::make_unique<window_dx12>(::GetModuleHandle(nullptr), title, width, height);
#endif
}

sge::window_interface::window_interface(const wchar_t* title, int width, int height) {
    mTitle = title;
    mSize.x() = width;
    mSize.y() = height;
}

void sge::window_interface::hide() {
    mState = state::hidden;
    mActive = false;
    on_hide();
    events::engine::on_window_closed.broadcast();
}

void sge::window_interface::show() {
    mState = state::normal;
    mActive = true;
    on_show();
    events::engine::on_window_opened.broadcast();
}

void sge::window_interface::restore() {
    mState = state::normal;
    mActive = true;
    on_restore();
    events::engine::on_window_focus_changed.broadcast({ true });
}

void sge::window_interface::minimize() {
    mState = state::minimized;
    mActive = false;
    on_minimize();
    events::engine::on_window_minimized.broadcast();
}

void sge::window_interface::maximize() {
    mState = state::maximized;
    mActive = true;
    on_maximize();
    events::engine::on_window_maximized.broadcast();
}

void sge::window_interface::resize(int width, int height) {
    mSize = int2::values(width, height);
    on_resize(width, height);
    events::engine::on_window_resized.broadcast({ width, height });
}

void sge::window_interface::move(int x, int y) {
    on_move(x, y);
    events::engine::on_window_moved.broadcast({ x, y });
}