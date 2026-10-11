#pragma once

#include "utility/defines.h"
#include "filesystem/filestream.h"

#include <EASTL/string_view.h>
#include <EASTL/array.h>
#include <EASTL/unique_ptr.h>

#include <format>
#include <cstdio>


namespace sge::logging {
    enum class level : uint8_t {
        trace,
        debug,
        info,
        warning,
        error,
        critical,
        _count
    };

    void set_log_level(level lvl);

    class logger {
    public:
        explicit logger(const char* name);
        explicit logger(const char* name, level lvl);
        ~logger();

        template <typename... Args>
        void log(level lvl, const char* fmt, Args&&... args) {
            if (!can_log(lvl)) { return; }
            std::string fmtMsg = std::vformat(fmt, std::make_format_args(args...));
            fmtMsg = std::format("{}{} :: {}", get_tag(lvl), mName, fmtMsg);

            std::fwrite(fmtMsg.data(), 1, fmtMsg.size(), stdout);
            std::fputc('\n', stdout);
            // Output to file
            if (mFilestream) {
                auto result = mFilestream->write(fmtMsg.c_str(), fmtMsg.size()); if (!result) { throw std::system_error(result.error()); }
                auto flushed = mFilestream->flush(); if (!flushed) { throw std::system_error(flushed.error()); }
            }

            // Output to file
        }

        template <typename... Args>
        void log_trace(const char* fmt, Args&&... args) {
            log(level::trace, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_debug(const char* fmt, Args&&... args) {
            log(level::debug, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_info(const char* fmt, Args&&... args) {
            log(level::info, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_warning(const char* fmt, Args&&... args) {
            log(level::warning, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_error(const char* fmt, Args&&... args) {
            log(level::error, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void log_critical(const char* fmt, Args&&... args) {
            log(level::critical, fmt, std::forward<Args>(args)...);
        }

        const std::string_view get_name() const {
            return mName;
        }

        void set_log_level(level lvl) {
            mLogLevel = lvl;
        }

        level get_log_level() const {
            return mLogLevel;
        }

    private:
        SGE_NODISCARD bool can_log(level lvl) const;
        SGE_NODISCARD std::string_view get_tag(level lvl) const;

    private:
        std::string mName;
        level mLogLevel = level::info;
        eastl::unique_ptr<filesystem::ofstream> mFilestream;
    };
}