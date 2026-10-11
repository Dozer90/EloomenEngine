#include "logging.h"

#include <EASTL/array.h>

#include <cstdarg>
#include <format>

using namespace sge;

namespace {
    logging::level currentLogLevel = logging::level::info; // Default log level

    std::string create_filename() { return "shine-engine.log"; }
}

logging::logger::logger(const char* name)
    : mName(name) {
    mFilestream = eastl::make_unique<filesystem::ofstream>(create_filename());
}

logging::logger::logger(const char* name, level lvl) : logger(name) { mLogLevel = lvl; }
logging::logger::~logger() = default;

void logging::set_log_level(level lvl) {
    currentLogLevel = lvl;
}

SGE_NODISCARD bool logging::logger::can_log(level lvl) const {
    return lvl >= currentLogLevel && lvl >= mLogLevel;
}

SGE_NODISCARD std::string_view logging::logger::get_tag(level lvl) const {
    static constexpr eastl::array<const char*, static_cast<uint8_t>(level::_count)> LOG_NAMES = {
        "[[ TRACE ]]     ",
        "[[ DEBUG ]]     ",
        "[[ INFO ]]      ",
        "[[ WARNING ]]   ",
        "[[ ERROR ]]     ",
        "[[ CRITICAL ]]  "
    };
    const uint8_t logLevelIndex = static_cast<uint8_t>(lvl);
    SGE_ASSERT(logLevelIndex < LOG_NAMES.size(), "Invalid log level index: {}", logLevelIndex);
    return LOG_NAMES[logLevelIndex];
}