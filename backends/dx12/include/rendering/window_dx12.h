#pragma once
#include "core/window_interface.h"

#include <EASTL/string.h>

#include <Windows.h>


namespace eloo {
    class window_dx12 : public window_interface {
    public:
        window_dx12(HINSTANCE instance, const eastl::wstring& title, int width, int height);
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
        static LRESULT CALLBACK WindowProc(HWND handle, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool process_command(UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HINSTANCE mInstance = nullptr;
        HWND mHandle = nullptr;
    };
}