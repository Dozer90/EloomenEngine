#include "rendering/renderer.h"
#include "rendering/window.h"

#include "datatypes/int2.h"


int main() {
    constexpr eloo::int2::values windowSize(1920, 1080);

    eastl::unique_ptr<eloo::window> wnd = eastl::make_unique<eloo::window>(L"Eloom Engine", windowSize.x(), windowSize.y());
    eastl::unique_ptr<eloo::renderer> rnd = eastl::make_unique<eloo::renderer>(wnd.get());
    while (wnd->process_messages()) {
        rnd->render();
    }
    return 0;
}