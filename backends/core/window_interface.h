#pragma once

#include "datatype/float2.h"
#include "datatype/int2.h"

#include <EASTL/functional.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/string.h>


namespace eloo {
    class window_interface {
    public:
        enum class state : int {
            invalid = -1,
            hidden,
            normal,
            minimized,
            maximized,
            _count
        };

        window_interface(const eastl::wstring& title, int width, int height);
        virtual ~window_interface() = default;

        const eastl::wstring& title() const { return mTitle; }
        const int2::values& size() const    { return mSize; }
        const float2::values& dpi() const   { return mDPI; }

        void hide();
        void show();
        void restore();
        void minimize();
        void maximize();
        void resize(int width, int height);
        void move(int x, int y);

        virtual void on_hide() {}
        virtual void on_show() {}
        virtual void on_restore() {}
        virtual void on_minimize() {}
        virtual void on_maximize() {}
        virtual void on_resize(int width, int height) {}
        virtual void on_move(int x, int y) {}

        inline bool is_active() const       { return mActive; }
        inline bool is_hidden() const       { return mState == state::hidden; }
        inline bool is_minimized() const    { return mState == state::minimized; }
        inline bool is_maximized() const    { return mState == state::maximized; }

        virtual bool process_messages() = 0;

    protected:
        eastl::wstring mTitle = L"";
        state mState = state::invalid;
        bool mActive = false;
        int2::values mSize = int2::ZERO;
        float2::values mDPI = float2::ONE;
    };
}