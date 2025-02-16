#pragma once

#include <EASTL/string.h>

#include <Windows.h>


namespace eloo {
class Window {
public:
    enum class State {
        invalid = -2,
        hidden = -1,
        normal,
        minimized,
        maximized,
        _count
    };

public:
    Window(HINSTANCE instance, const eastl::wstring title, int width, int height);
    ~Window();

    void hide();
    void show();
    void restore();
    void minimize();
    void maximize();

    inline bool isActive() const        { return mActive; }
    inline bool isMinimized() const     { return mState != State::minimized; }
    inline bool isMaximized() const     { return mState != State::maximized; }

    inline HWND handle() const          { return mHandle; }

    bool processMessages();

private:
    static LRESULT CALLBACK WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool processCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    eastl::wstring mTitle = L"";
    HINSTANCE mInstance = nullptr;
    HWND mHandle = nullptr;
    int mWidth;
    int mHeight;
    bool mActive = false;
    State mState = State::invalid;
};
}