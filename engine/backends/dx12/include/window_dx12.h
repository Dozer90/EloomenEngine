#pragma once

#include "window.h"

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
    class window : public eloo::window<eloo::dx12::window> {
    public:
        window(HINSTANCE instance,
               const wchar_t* title,
               int width,
               int height);
        ~window();

    private:
        void hide();
        void show();
        void restore();
        void minimize();
        void maximize();
        void resize(int width, int height);
        void move(int x, int y);

        inline HWND handle() const { return mHandle; }

        bool process_messages();

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
