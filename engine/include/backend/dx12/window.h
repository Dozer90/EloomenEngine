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
        eloo::core::void_r hide_impl();
        eloo::core::void_r show_impl();
        eloo::core::void_r restore_impl();
        eloo::core::void_r minimize_impl();
        eloo::core::void_r maximize_impl();
        eloo::core::void_r resize_impl(int width, int height);
        eloo::core::void_r set_position_impl(int x, int y);

        inline HWND handle() const { return mHandle; }

        eloo::core::void_r process_messages_impl();

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
