#pragma once

#include "window_interface.h"

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

namespace eloo {
    class window_dx12 : public window_interface {
    public:
        window_dx12(HINSTANCE instance,
                    const eastl::wstring& title,
                    int width,
                    int height);
        ~window_dx12();

        void on_hide() override;
        void on_show() override;
        void on_restore() override;
        void on_minimize() override;
        void on_maximize() override;
        void on_resize(int width, int height) override;
        void on_move(int x, int y) override;

        inline HWND handle() const { return mHandle; }

        bool process_messages() override;

    private:
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
