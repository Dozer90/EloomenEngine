#pragma once

#include "error_codes/rendering_errc.h"

#include "events/engine/events.h"

#include "datatypes/float2.h"
#include "datatypes/int2.h"


namespace eloo::core {
    using void_r = std::expected<void, std::error_code>;
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
            mSize = int2::create(width, height);
            mDPI = float2::create(1.0f, 1.0f); // Default DPI scaling to 100%
        }
        window(const window&) = delete;
        window& operator=(const window&) = delete;
        window(window&&) = delete;
        window& operator=(window&&) = delete;

    public:
        const wchar_t* get_title() const    { return mTitle; }
        const int get_width() const         { return int2::const_x(mSize); }
        const int get_height() const        { return int2::const_y(mSize); }
        const float get_dpi_x() const       { return float2::const_x(mDPI); }
        const float get_dpi_y() const       { return float2::const_y(mDPI); }

        void_r set_active(bool active) {
            mActive = active;
            return {};
        }

        void_r set_state(state newState) {
            if (mState == newState) {
                return {}; // No state change needed
            }

            void_r result = std::unexpected(rendering::errc::unsupported_operation);
            switch (mState) {
                case state::hidden: {
                    result = ELOO_CRPT_CALL_T->show_impl();
                    break;
                }
                case state::normal: {
                    if (newState == state::hidden) {
                        result = ELOO_CRPT_CALL_T->hide_impl();
                    } else if (newState == state::minimized) {
                        result = ELOO_CRPT_CALL_T->minimize_impl();
                    } else if (newState == state::maximized) {
                        result = ELOO_CRPT_CALL_T->maximize_impl();
                    }
                    break;
                }
                case state::minimized:
                case state::maximized: {
                    if (newState == state::normal) {
                        result = ELOO_CRPT_CALL_T->restore_impl();
                    }
                    break;
                }
            }

            if (result) {
                mState = newState;
            }
            return result;
        }

        void_r resize(int width, int height) {
            auto result = ELOO_CRPT_CALL_T->resize_impl(width, height);
            if (result) {
                int2::set(mSize, width, height);
                ELOO_BROADCAST_EVENT_WITH_DATA(engine, window_resized, width, height);
            }
            return result;
        }

        void_r set_position(int x, int y, float xPivot, float yPivot) {
            int adjustedX = x + static_cast<int>(int2::x(mSize) * xPivot);
            int adjustedY = y + static_cast<int>(int2::y(mSize) * yPivot);
            auto result = ELOO_CRPT_CALL_T->set_position_impl(adjustedX, adjustedY);
            if (result) {
                int2::set(mPosition, adjustedX, adjustedY);
                ELOO_BROADCAST_EVENT_WITH_DATA(engine, window_moved, adjustedX, adjustedY);
            }
            return result;
        }

        inline bool is_active() const       { return mActive; }
        inline bool is_hidden() const       { return mState == state::hidden; }
        inline bool is_minimized() const    { return mState == state::minimized; }
        inline bool is_maximized() const    { return mState == state::maximized; }

        void_r process_messages() {
            return ELOO_CRPT_CALL_T->process_messages_impl();
        }

    protected:
        const wchar_t* mTitle = L"";
        state mState = state::invalid;
        bool mActive = false;
        int2::id_t mSize;
        int2::id_t mPosition;
        float2::id_t mDPI;
    };
}