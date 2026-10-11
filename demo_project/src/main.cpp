#include "rendering/renderer.h"
#include "rendering/window.h"

#include "datatypes/int2.h"


int main() {
    constexpr sge::int2::values windowSize(1920, 1080);

    sge::window_ptr wnd = sge::create_window(L"Shine Game Engine", windowSize.x(), windowSize.y());
    sge::renderer_ptr rnd = sge::create_renderer(wnd.get());
    while (wnd->process_messages()) {
        rnd->render(wnd.get());
    }
    return 0;
}