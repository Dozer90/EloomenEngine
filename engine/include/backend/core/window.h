#pragma once

#include "datatypes/float2.h"
#include "datatypes/int2.h"


namespace eloo::core {
    template <typename T>
    class window {
        friend T;

    public:
        enum class state : int {
            invalid = -1,
            hidden,
            normal,
            minimized,
            maximized,
            COUNT
        };

    protected:
        explicit window(const wchar_t* title, int width, int height) {
            mTitle = title;
            mSize = int2::values(width, height);
            mDPI = float2::values(1.0f, 1.0f); // Default DPI scaling to 100%
        }
        window(const window&) = delete;
        window& operator=(const window&) = delete;
        window(window&&) = delete;
        window& operator=(window&&) = delete;

    public:
        const wchar_t* title() const        { return mTitle; }
        const int2::values& size() const    { return mSize; }
        const float2::values& dpi() const   { return mDPI; }

        bool set_active(bool active) {
            mActive = active;
            return true;
        }

        bool set_state(state newState) {
            if (mState == newState) {
                return false; // No state change
            }

            bool result = false;
            switch (mState) {
                case state::hidden: {
                    result = ELOO_CRPT_CALL_T->show();
                    break;
                }
                case state::normal: {
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

        bool resize(int width, int height) {
            return ELOO_CRPT_CALL_T->resize(width, height);
        }

        bool set_position(int x, int y, float xPivot, float yPivot) {
            return ELOO_CRPT_CALL_T->set_position(x, y, xPivot, yPivot);
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