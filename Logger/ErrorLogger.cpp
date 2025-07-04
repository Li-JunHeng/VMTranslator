#include "ErrorLogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>

namespace Logger {

void ErrorLogger::IOError(const std::runtime_error& e, int lineNumber, const std::string& path) {
    std::cerr << "[ERROR] I/O Error: " << e.what() << std::endl;
    if (!path.empty()) {
        std::cerr << "  File: " << path << std::endl;
    }
    if (lineNumber != -1) {
        std::cerr << "  Line: " << lineNumber << std::endl;
    }
}

void ErrorLogger::cmdLogError(const std::invalid_argument& e, int lineNumber, const std::string& command) {
    std::cerr << "[ERROR] Command Parsing Error: " << e.what() << std::endl;
    if (lineNumber != -1) {
        std::cerr << "  Line: " << lineNumber << std::endl;
    }
    if (!command.empty()) {
        std::cerr << "  Command: \"" << command << "\"" << std::endl;
    }
}

void ErrorLogger::transLogError(const std::exception& e, const std::string& context) {
    std::cerr << "[ERROR] Translation Error: " << e.what() << std::endl;
    if (!context.empty()) {
        std::cerr << "  Context: " << context << std::endl;
    }
}

void ErrorLogger::logError(const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::cerr << "[ERROR] [" << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;
}

} // namespace Logger