#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Logger {

// 日志级别
enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// 设置日志级别
void setLogLevel(LogLevel level);

// 获取当前日志级别
LogLevel getLogLevel();

// 日志输出函数
void log(LogLevel level, const std::string& message);

// 便捷日志宏
#define LOG_INFO(message) if (Logger::getLogLevel() <= Logger::LogLevel::INFO) Logger::log(Logger::LogLevel::INFO, message)
#define LOG_WARNING(message) if (Logger::getLogLevel() <= Logger::LogLevel::WARNING) Logger::log(Logger::LogLevel::WARNING, message)
#define LOG_ERROR(message) if (Logger::getLogLevel() <= Logger::LogLevel::ERROR) Logger::log(Logger::LogLevel::ERROR, message)
#define LOG_FATAL(message) if (Logger::getLogLevel() <= Logger::LogLevel::FATAL) Logger::log(Logger::LogLevel::FATAL, message)

} // namespace Logger

#endif // LOGGER_H