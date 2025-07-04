#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <mutex>

namespace Logger {

static LogLevel currentLevel = LogLevel::INFO;
static std::mutex logMutex;

void setLogLevel(LogLevel level) {
    currentLevel = level;
}

LogLevel getLogLevel() {
    return currentLevel;
}

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void log(LogLevel level, const std::string& message) {
    if (level < currentLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);
    std::time_t now = std::time(nullptr);
    std::cout << "[" << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << std::setw(7) << logLevelToString(level) << "]: "
              << message << std::endl;
}

} // namespace Logger