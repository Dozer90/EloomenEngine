#include <Rendering/Window.h>

#include <Maths/float2.h>

using namespace eloo;
using namespace eloo::math;

Window::Window(HINSTANCE instance, const eastl::wstring title, int width, int height) :
    mInstance(instance), mTitle(title), mWidth(width), mHeight(height) {

    // Get DPI
    HDC screen = GetDC(0);
    mDPI = { GetDeviceCaps(screen, LOGPIXELSX), GetDeviceCaps(screen, LOGPIXELSY) } / 96.0f;
    ReleaseDC(0, screen);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = mInstance;
    wc.lpszClassName = L"DX12Window";
    RegisterClassExW(&wc);

    mHandle = CreateWindowExW(
        0,
        wc.lpszClassName,
        mTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        mWidth, mHeight,
        nullptr, nullptr,
        mInstance, nullptr
    );

    SetWindowLongPtrW(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(mHandle, SW_SHOWNORMAL);
    mState = State::normal;
}

void Window::show() {
    if (!mActive && mState < State::normal) return;
    mState = State::normal;
    ShowWindow(mHandle, SW_SHOW);
}

void Window::hide() {
    if (mActive && mState >= State::normal) return;
    mState = State::hidden;
    ShowWindow(mHandle, SW_HIDE);
}

void Window::restore() {
    if (mActive && mState != State::normal) return;
    mState = State::normal;
    ShowWindow(mHandle, SW_RESTORE);
}

void Window::minimize() {
    if (mActive && mState != State::minimized) return;
    mState = State::minimized;
    ShowWindow(mHandle, SW_MINIMIZE);
}

void Window::maximize() {
    if (mActive && mState != State::maximized) return;
    mState = State::maximized;
    ShowWindow(mHandle, SW_MAXIMIZE);
}

bool Window::processMessages() {
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

bool Window::processCommand(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CLOSE:
        DestroyWindow(mHandle);
        return true;

    case WM_DESTROY:
        PostQuitMessage(0);
        return true;

    case WM_SIZE:
        mWidth = LOWORD(lParam);
        mHeight = HIWORD(lParam);
        return true;

    case WM_ACTIVATE:
        mActive = LOWORD(wParam) != WA_INACTIVE;
        return false;

    case WM_PAINT:
        ValidateRect(mHandle, nullptr);
        return false;
    }

    return false;
}

LRESULT CALLBACK Window::WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
    if (window != nullptr && window->processCommand(uMsg, wParam, lParam)) {
        return 0;
    }
    return DefWindowProc(handle, uMsg, wParam, lParam);
}