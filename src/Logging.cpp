#include "Logging.h"

#include <EASTL/string.h>

#include <fstream>
#include <filesystem>
#include <ctime>
#include <iostream>

constexpr size_t MAX_LOGFILE_COUNT = 5;

using namespace eloo::Logging;

namespace {
eastl::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto nowt = std::chrono::system_clock::to_time_t(now);
    std::tm time_tm = *std::localtime(&nowt);

    std::ostringstream oss;
    oss << std::put_time(&time_tm, "%Y-%m-%d");
    return eastl::string(oss.str().c_str());
}

eastl::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto nowt = std::chrono::system_clock::to_time_t(now);
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&nowt), "%H:%M:$S");
    oss << '.' << std::setfill('0') << std::setw(3) << milli.count();
    return eastl::string(oss.str().c_str());
}

const char* getTypeString(LogType type) {
    switch (type) {
    case LogType::Debug:   return "[Debug]";
    case LogType::Info:    return "[Info]";
    case LogType::Trace:   return "[Trace]";
    case LogType::Warning: return "[Warning]";
    case LogType::Error:   return "[ERROR]";
    case LogType::Fatal:   return "[FATAL]";
    }
    return "";
}

bool isTypeAllowed(LogType type) {
#if !defined(DEBUG_BUILD)
    if (type == LogType::Debug) {
        return false;
    }
#endif
#if !defined(VERBOSE_LOGGING)
    if (type == LogType::Trace) {
        return false;
    }
#endif
    return true;
}
}

Logger::Logger(const std::filesystem::path& directory, const char* filename) {
    std::filesystem::path file = directory / (eastl::string().sprintf("%s_%s.log", filename, getCurrentDate())).c_str();
    mLogFile.open(file, std::ios::app);

    if (mLogFile.is_open()) {
        // Get all log files in the log directory
        std::vector<std::filesystem::path> logFiles;
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".log") {
                if (entry.path().stem().string().find(filename) == 0) {
                    logFiles.push_back(entry.path());
                }
            }
        }

        // Sort log files by last write time (oldest first)
        std::sort(logFiles.begin(), logFiles.end(),
                  [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
        });

        // Remove the oldest files if we exceed the limit
        if (logFiles.size() > MAX_LOGFILE_COUNT) {
            for (size_t i = 0; i < logFiles.size() - MAX_LOGFILE_COUNT; ++i) {
                std::filesystem::remove(logFiles[i]);
            }
        }
    }
}

Logger::~Logger() {
    mLogFile.close();
}

void Logger::log(LogType type, const char* message, ...) {
    if (isTypeAllowed(type)) {
        return;
    }

    eastl::string msg = getTimestamp();
    msg.append_sprintf(" %12s", getTypeString(type));

    va_list args;
    va_start(args, message);
    msg.append_sprintf_va_list(message, args);
    va_end(args);

    std::cout << msg.c_str() << std::endl;
    if (mLogFile.is_open()) {
        mLogFile << msg.c_str() << std::endl;
    }
}

void Logger::logDebug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Debug, message, args);
    va_end(args);
}

void Logger::logInfo(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Info, message, args);
    va_end(args);
}

void Logger::logTrace(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Trace, message, args);
    va_end(args);
}

void Logger::logWarning(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Warning, message, args);
    va_end(args);
}

void Logger::logError(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Error, message, args);
    va_end(args);
}

void Logger::logFatal(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(LogType::Fatal, message, args);
    va_end(args);
}