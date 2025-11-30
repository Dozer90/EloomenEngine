#pragma once

#include "datatypes/float2.h"
#include "datatypes/int2.h"

#include <EASTL/unique_ptr.h>


namespace eloo {
    template <typename T>
    class window {

    public:
        enum class state : int {
            invalid = -1,
            hidden,
            normal,
            minimized,
            maximized,
            COUNT
        };

    public:
        window(const wchar_t* title, int width, int height) {
            mTitle = title;
            mSize = int2::values(width, height);
            mDPI = float2::values(1.0f, 1.0f); // Default DPI scaling to 100%
        }

        const wchar_t* title() const    { return mTitle; }
        const int2::values& size()      { return mSize; }
        const float2::values& dpi()     { return mDPI; }

        bool set_state(state newState) {
            if (mState == newState) {
                return false; // No state change
            }

            bool result = false;
            switch (mState) {
                case state::hidden: {
                    if (newState == window::state::normal) {
                        result = ELOO_CRPT_CALL_T->show();
                    }
                    break;
                }
                case window::state::normal: {
                    if (newState == state::hidden) {
                        result = ELOO_CRPT_CALL_T->hide();
                    } else if (newState == state::minimized) {
                        result = ELOO_CRPT_CALL_T->minimize();
                    } else if (newState == state::maximized) {
                        result = ELOO_CRPT_CALL_T->maximize();
                    }
                    break;
                }
                case state::minimized:
                case state::maximized: {
                    if (newState == state::normal) {
                        result = ELOO_CRPT_CALL_T->restore();
                    }
                    break;
                }
            }

            if (result) {
                mState = newState;
            }
            return result;
        }

        bool set_active(bool active) {
            if (active != mActive) {
                if (ELOO_CRPT_CALL_T->set_active(active)) {
                    mActive = active;
                    return true;
                }
            }
            return false;
        }

        inline bool is_active() const       { return mActive; }
        inline bool is_hidden() const       { return mState == state::hidden; }
        inline bool is_minimized() const    { return mState == state::minimized; }
        inline bool is_maximized() const    { return mState == state::maximized; }

        bool process_messages() {
            return ELOO_CRPT_CALL_T->process_messages();
        }

    protected:
        const wchar_t* mTitle = L"";
        state mState = state::invalid;
        bool mActive = false;
        int2::values mSize = int2::ZERO;
        float2::values mDPI = float2::ONE;
    };
}