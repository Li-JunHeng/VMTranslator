#include "OutputHelper.h"
#include <stdexcept>
#include "../Logger/ErrorLogger.h"

using namespace std;
namespace IO_Helper {

OutputHelper::OutputHelper(const string& filename) : outputStream(&fileStream), ownsStream(true) {
    fileStream.open(filename);
    if (!fileStream.is_open()) {
        Logger::ErrorLogger::IOError(runtime_error("无法打开输出文件"), -1, filename);
        outputStream = nullptr;
    }
}

OutputHelper::OutputHelper(ostream& stream) : outputStream(&stream), ownsStream(false) {
    if (!stream.good()) {
        Logger::ErrorLogger::IOError(runtime_error("输出流状态无效"), -1, "");
        outputStream = nullptr;
    }
}

OutputHelper::~OutputHelper() {
    close();
}

bool OutputHelper::isValid() const {
    return outputStream != nullptr && (*outputStream).good();
}

void OutputHelper::writeLine(const string& line) {
    if (isValid()) {
        *outputStream << line << '\n';
        if (!outputStream->good()) {
            Logger::ErrorLogger::IOError(runtime_error("写入行时发生错误"), -1, "");
        }
    }
}

void OutputHelper::writeLines(const vector<string>& lines) {
    for (const string& line : lines) {
        writeLine(line);
    }
}

void OutputHelper::close() {
    if (ownsStream && fileStream.is_open()) {
        fileStream.close();
    }
}

} // namespace IO_Helper