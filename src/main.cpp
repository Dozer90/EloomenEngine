#include "rendering/renderer.h"
#include "rendering/window.h"


int main() {
    constexpr eloo::int2::values windowSize(1920, 1080);

    eloo::window_ptr wnd = eloo::create_window(L"Eloom Engine", windowSize.x(), windowSize.y());
    eloo::renderer_ptr rnd = eloo::create_renderer(wnd.get());
    while (wnd->process_messages()) {
        rnd->render();
    }
    return 0;
}