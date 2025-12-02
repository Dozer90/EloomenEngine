#pragma once

#include "backend/core/window.h"

#include <EASTL/string.h>

#include <cstdint>   // for intptr_t/uintptr_t

// Win32 handle-types as opaque structs
struct HWND__;
using HWND = HWND__*;
struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;

// Win32 message-proc types as integer aliases
using UINT    = unsigned int;
using WPARAM  = std::uintptr_t;
using LPARAM  = std::intptr_t;
using LRESULT = std::intptr_t;

namespace eloo::dx12 {
    class window : public eloo::core::window<window> {
    public:
        explicit window(const wchar_t* title, int width, int height);
        ~window();

    private:
        bool hide_impl();
        bool show_impl();
        bool restore_impl();
        bool minimize_impl();
        bool maximize_impl();
        bool resize(int width, int height);
        bool move(int x, int y, float xPivot = 0.0f, float yPivot = 0.0f);

        inline HWND handle() const { return mHandle; }

        bool process_messages_impl();

        static LRESULT __stdcall
        WindowProc(HWND   handle,
                   UINT   uMsg,
                   WPARAM wParam,
                   LPARAM lParam);

        bool process_command(UINT   uMsg,
                            WPARAM wParam,
                            LPARAM lParam);

    private:
        HINSTANCE mInstance = nullptr;
        HWND      mHandle   = nullptr;
    };
}
