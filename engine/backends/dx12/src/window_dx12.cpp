#include "dx12/include/window_dx12.h"

#include "rendering/window.h"
#include "utility/defines.h"

#include <EASTL/string.h>

#include <Windows.h>

using namespace eloo;

namespace {
    constexpr LPCWCH WINDOW_CLASS_NAME = L"EloomEngineWindow";
}

window_dx12::window_dx12(HINSTANCE instance, const eastl::wstring& title, int width, int height) :
    mInstance(instance), window_interface(title, width, height) {

    // Get DPI
    HDC screen = GetDC(0);
    mDPI.x() = GetDeviceCaps(screen, LOGPIXELSX) / 96.0f;
    mDPI.y() = GetDeviceCaps(screen, LOGPIXELSY) / 96.0f;
    ReleaseDC(0, screen);

    // Ensure the window class is registered
    static bool registered = false;
    if (!registered) {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = Window::WindowProc;
        wc.hInstance = mInstance;
        wc.lpszClassName = WINDOW_CLASS_NAME;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        if (!RegisterClassExW(&wc)) {
            ELOO_ASSERT_FATAL_FALSE("Failed to register window class");
            return;
        }
        registered = true;
    }

    // Adjust window size to DPI
    RECT rect = {
        0,
        0,
        static_cast<LONG>(mSize.x() * mDPI.x()),
        static_cast<LONG>(mSize.y() * mDPI.y())
    };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    mHandle = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        mTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        mInstance,
        this
    );

    if (mHandle == nullptr) {
        ELOO_ASSERT_FATAL_FALSE("Failed to create window");
        return;
    }

    // Link user data
    SetWindowLongPtrW(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(mHandle, SW_SHOWNORMAL);
    mState = state::normal;
    mActive = true;
}

window_dx12::~window_dx12() {
    if (mHandle != nullptr) {
        DestroyWindow(mHandle);
        mHandle = nullptr;
    }
}

void window_dx12::on_show() {
    ShowWindow(mHandle, SW_SHOW);
}

void window_dx12::on_hide() {
    ShowWindow(mHandle, SW_HIDE);
}

void window_dx12::on_restore() {
    ShowWindow(mHandle, SW_RESTORE);
}

void window_dx12::on_minimize() {
    ShowWindow(mHandle, SW_MINIMIZE);
}

void window_dx12::on_maximize() {
    ShowWindow(mHandle, SW_MAXIMIZE);
}

void window_dx12::on_resize(int width, int height) {
    mSize = int2::values(width, height);
    SetWindowPos(mHandle, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void window_dx12::on_move(int x, int y) {
    SetWindowPos(mHandle, nullptr, x, y, mSize.x(), mSize.y(), SWP_NOZORDER | SWP_NOSIZE);
}

bool window_dx12::process_messages() {
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        if (msg.message == WM_QUIT) {
            return false;
        }
    }
    return true;
}

bool window_dx12::process_command(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: {
            DestroyWindow(mHandle);
            return true;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return true;
        }
        case WM_SIZE: {
            resize(LOWORD(lParam), HIWORD(lParam));
            return true;
        }
        case WM_ACTIVATE: {
            mActive = (LOWORD(wParam) != WA_INACTIVE);
            return false;
        }
        case WM_PAINT: {
            ValidateRect(mHandle, nullptr);
            return false;
        }
    }

    return false;
}

LRESULT CALLBACK window_dx12::WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        // Store the window pointer in user data
        CREATESTRUCTW* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
    }
    Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
    if (wnd != nullptr && wnd->process_command(uMsg, wParam, lParam)) {
        return 0;
    }
    return DefWindowProc(handle, uMsg, wParam, lParam);
}