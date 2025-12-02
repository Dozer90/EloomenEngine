#include "backend/dx12/window.h"

#include "utility/defines.h"

#include <Windows.h>

using namespace eloo;

namespace {
    constexpr LPCWCH WINDOW_CLASS_NAME = L"EloomEngineWindow";
}

dx12::window::window(const wchar_t* title, int width, int height) :
  mInstance(GetModuleHandle(nullptr)),
  eloo::core::window<dx12::window>(title, width, height) {

    // Get DPI
    HDC screen = GetDC(0);
    mDPI = float2::create(
        GetDeviceCaps(screen, LOGPIXELSX) / 96.0f,
        GetDeviceCaps(screen, LOGPIXELSY) / 96.0f);
    ReleaseDC(0, screen);

    // Ensure the window class is registered
    static bool registered = false;
    if (!registered) {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = dx12::window::WindowProc;
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
        static_cast<LONG>(int2::x(mSize) * float2::x(mDPI)),
        static_cast<LONG>(int2::y(mSize) * float2::y(mDPI))
    };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    mHandle = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        mTitle,
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

dx12::window::~window() {
    if (mHandle != nullptr) {
        DestroyWindow(mHandle);
        mHandle = nullptr;
    }
}

eloo::core::void_r dx12::window::show_impl() {
    ShowWindow(mHandle, SW_SHOW);
    return {};
}

eloo::core::void_r dx12::window::hide_impl() {
    ShowWindow(mHandle, SW_HIDE);
    return {};
}

eloo::core::void_r dx12::window::restore_impl() {
    ShowWindow(mHandle, SW_RESTORE);
    return {};
}

eloo::core::void_r dx12::window::minimize_impl() {
    ShowWindow(mHandle, SW_MINIMIZE);
    return {};
}

eloo::core::void_r dx12::window::maximize_impl() {
    ShowWindow(mHandle, SW_MAXIMIZE);
    return {};
}

eloo::core::void_r dx12::window::resize_impl(int width, int height) {
    if (width <= 0 || height <= 0) {
        return std::unexpected(rendering::errc::resize_failed);
    }

    if (is_active() && !is_hidden() && !is_maximized() && !is_minimized()) {
        SetWindowPos(mHandle, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
        return {};
    }
    return std::unexpected(rendering::errc::unsupported_operation);
}

eloo::core::void_r dx12::window::set_position_impl(int x, int y) {
    SetWindowPos(mHandle, nullptr, x, y, get_width(), get_height(), SWP_NOZORDER | SWP_NOSIZE);
    return {};
}

eloo::core::void_r dx12::window::process_messages_impl() {
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        if (msg.message == WM_QUIT) {
            return std::unexpected(rendering::errc::unsupported_operation); // Or create a "window_closed" error
        }
    }
    return {};
}

bool dx12::window::process_command(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
            set_active(LOWORD(wParam) != WA_INACTIVE);
            return false;
        }
        case WM_PAINT: {
            ValidateRect(mHandle, nullptr);
            return false;
        }
    }

    return false;
}

LRESULT CALLBACK dx12::window::WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_NCCREATE) {
        // Store the window pointer in user data
        CREATESTRUCTW* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
    }
    dx12::window* wnd = reinterpret_cast<dx12::window*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
    if (wnd != nullptr && wnd->process_command(uMsg, wParam, lParam)) {
        return 0;
    }
    return DefWindowProc(handle, uMsg, wParam, lParam);
}