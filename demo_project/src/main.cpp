#include "rendering/renderer.h"
#include "rendering/window.h"

#include "datatypes/int2.h"


int main() {
    constexpr eloo::int2::values windowSize(1920, 1080);

    eloo::window* wnd = new eloo::window(L"Eloom Engine", windowSize.x(), windowSize.y());
    eloo::renderer* rnd = new eloo::renderer(wnd);
    while (wnd->process_messages()) {
        rnd->render();
    }
    return 0;
}