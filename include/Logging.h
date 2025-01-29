#pragma once

namespace std { class ofstream; }
namespace std::filesystem { class path; }

namespace eloo::Logging {

enum class LogType {
    Debug,      // Debug messages
    Info,       // General information
    Trace,      // Detailed information
    Warning,    // Warnings
    Error,      // Errors
    Fatal,      // Fatal errors
};

class Logger {
public:
    Logger(const std::filesystem::path& directory, const char* filename);
    ~Logger();

    void log(LogType type, const char* message, ...);

    void logDebug(const char* message, ...);
    void logInfo(const char* message, ...);
    void logTrace(const char* message, ...);
    void logWarning(const char* message, ...);
    void logError(const char* message, ...);
    void logFatal(const char* message, ...);

private:
    std::ofstream mLogFile;
};
}