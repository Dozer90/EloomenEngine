#pragma once

#include "utility/defines.h"
#include "filesystem/filestream.h"

#include <EASTL/string_view.h>
#include <EASTL/array.h>
#include <EASTL/unique_ptr.h>

#include <format>
#include <cstdio>


namespace eloo::logging {
    enum class level : uint8_t {
        trace,
        debug,
        info,
        warning,
        error,
        critical,
        _count
    };

    void set_log_level(level lvl) ELOO_NOEXCEPT;

    class logger {
    public:
        explicit logger(const char* name) ELOO_NOEXCEPT;
        explicit logger(const char* name, level lvl) ELOO_NOEXCEPT;
        ~logger() ELOO_NOEXCEPT;

        template <typename... Args>
        void log(level lvl, const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            std::string fmtMsg = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
            fmtMsg = std::format("{}{} :: {}", get_tag(lvl), mName, fmtMsg);

            // Output to console
            if (mFile) {
                mFile->write(fmtMsg.c_str(), fmtMsg.size());
                mFile->flush();
            }

            // Output to file
        }

        template <typename... Args>
        void log_trace(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::trace, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_debug(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::debug, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_info(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::info, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_warning(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::warning, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_error(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::error, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_critical(const char* fmt, Args&&... args) ELOO_NOEXCEPT {
            log(level::critical, fmt, std::forward<Args>(args)...);
        }

        const std::string_view get_name() const ELOO_NOEXCEPT {
            return mName;
        }

        void set_log_level(level lvl) ELOO_NOEXCEPT {
            mLogLevel = lvl;
        }

        level get_log_level() const ELOO_NOEXCEPT {
            return mLogLevel;
        }

    private:
        ELOO_NODISCARD bool can_log(level lvl) const ELOO_NOEXCEPT;
        ELOO_NODISCARD std::string_view get_tag(level lvl) const ELOO_NOEXCEPT;

    private:
        std::string mName;
        level mLogLevel = level::info;
        eastl::unique_ptr<filesystem::ofstream> mFilestream;
    };
}