#include "logging.h"

#include <EASTL/array.h>

#include <cstdarg>
#include <format>

using namespace eloo;

namespace {
    logging::level currentLogLevel = logging::level::info; // Default log level

    const char* create_filename() {
        eastl::chrono::system_clock::time_point now = eastl::chrono::system_clock::now();
    }
}

logging::logger::logger(const char* name) ELOO_NOEXCEPT
    : mName(name) {
    mFilestream = eastl::make_unique<filesystem::ofstream>(create_filename());
}

void logging::set_log_level(level lvl) ELOO_NOEXCEPT {
    currentLogLevel = lvl;
}

ELOO_NODISCARD bool logging::logger::can_log(level lvl) const ELOO_NOEXCEPT {
    return lvl >= currentLogLevel && lvl >= mLogLevel;
}

ELOO_NODISCARD std::string_view logging::logger::get_tag(level lvl) const ELOO_NOEXCEPT {
    static constexpr eastl::array<const char*, static_cast<uint8_t>(level::_count)> LOG_NAMES = {
        "[[ TRACE ]]     ",
        "[[ DEBUG ]]     ",
        "[[ INFO ]]      ",
        "[[ WARNING ]]   ",
        "[[ ERROR ]]     ",
        "[[ CRITICAL ]]  "
    };
    const uint8_t logLevelIndex = static_cast<uint8_t>(lvl);
    ELOO_ASSERT(logLevelIndex < LOG_NAMES.size(), "Invalid log level index: {}", logLevelIndex);
    return LOG_NAMES[logLevelIndex];
}