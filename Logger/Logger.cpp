#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <mutex>

namespace Logger {

using namespace std;
static LogLevel currentLevel = LogLevel::INFO;
static mutex logMutex;

void setLogLevel(LogLevel level) {
    currentLevel = level;
}

LogLevel getLogLevel() {
    return currentLevel;
}

string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void log(LogLevel level, const string& message) {
    if (level < currentLevel) return;

    lock_guard<mutex> lock(logMutex);
    time_t now = time(nullptr);
    cout << "[" << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << setw(7) << logLevelToString(level) << "]: "
              << message << endl;
}

} // namespace Logger