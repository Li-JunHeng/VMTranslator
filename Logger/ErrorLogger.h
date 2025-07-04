#pragma once

#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <string>
#include <stdexcept>

namespace Logger {

class ErrorLogger {
public:
    // 记录I/O错误
    static void IOError(const std::runtime_error& e, int lineNumber, const std::string& path);
    // 记录命令解析错误
    static void cmdLogError(const std::invalid_argument& e, int lineNumber, const std::string& command);
    // 记录翻译错误
    static void transLogError(const std::exception& e, const std::string& context);
    // 记录通用错误
    static void logError(const std::string& message);
};

} // namespace Logger

#endif // ERRORLOGGER_H