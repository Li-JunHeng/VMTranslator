#include "OutputHelper.h"
#include <stdexcept>
#include "../Logger/ErrorLogger.h"

namespace IO_Helper {

OutputHelper::OutputHelper(const std::string& filename) : outputStream(&fileStream), ownsStream(true) {
    fileStream.open(filename);
    if (!fileStream.is_open()) {
        Logger::ErrorLogger::IOError(std::runtime_error("无法打开输出文件"), -1, filename);
        outputStream = nullptr;
    }
}

OutputHelper::OutputHelper(std::ostream& stream) : outputStream(&stream), ownsStream(false) {
    if (!stream.good()) {
        Logger::ErrorLogger::IOError(std::runtime_error("输出流状态无效"), -1, "");
        outputStream = nullptr;
    }
}

OutputHelper::~OutputHelper() {
    close();
}

bool OutputHelper::isValid() const {
    return outputStream != nullptr && (*outputStream).good();
}

void OutputHelper::writeLine(const std::string& line) {
    if (isValid()) {
        *outputStream << line << '\n';
        if (!outputStream->good()) {
            Logger::ErrorLogger::IOError(std::runtime_error("写入行时发生错误"), -1, "");
        }
    }
}

void OutputHelper::writeLines(const std::vector<std::string>& lines) {
    for (const std::string& line : lines) {
        writeLine(line);
    }
}

void OutputHelper::close() {
    if (ownsStream && fileStream.is_open()) {
        fileStream.close();
    }
}

} // namespace IO_Helper