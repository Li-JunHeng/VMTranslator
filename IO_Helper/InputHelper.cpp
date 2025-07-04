#include "InputHelper.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include "../Logger/ErrorLogger.h"

namespace IO_Helper {

// 辅助函数：修剪字符串首尾空白字符
static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

InputHelper::InputHelper(const std::string& filename) : inputStream(&fileStream), ownsStream(true), lineNumber(0) {
    fileStream.open(filename);
    if (!fileStream.is_open()) {
        Logger::ErrorLogger::IOError(std::runtime_error("无法打开输入文件"), -1, filename);
        inputStream = nullptr;
    }
}

InputHelper::InputHelper(std::istream& stream) : inputStream(&stream), ownsStream(false), lineNumber(0) {
    if (!stream.good()) {
        Logger::ErrorLogger::IOError(std::runtime_error("输入流状态无效"), -1, "");
        inputStream = nullptr;
    }
}

InputHelper::InputHelper(const InputHelper& other) : inputStream(other.inputStream), ownsStream(false), lineNumber(other.lineNumber) {
    // 拷贝构造函数不拥有流所有权
}

InputHelper::~InputHelper() {
    if (ownsStream && fileStream.is_open()) {
        fileStream.close();
    }
}

bool InputHelper::isValid() const {
    return inputStream != nullptr && (*inputStream).good();
}

std::pair<std::string, int> InputHelper::readLine() {
    if (!isValid()) {
        return {"", -1};
    }

    std::string line;
    if (std::getline(*inputStream, line)) {
        lineNumber++;
        // 移除行内注释
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        // 修剪空白字符
        std::string trimmedLine = trim(line);
        return {trimmedLine, lineNumber};
    }

    // 到达文件末尾
    return {"", -1};
}

std::vector<std::pair<std::string, int>> InputHelper::readAllLines() {
    std::vector<std::pair<std::string, int>> lines;
    std::pair<std::string, int> line;
    while (!(line = readLine()).first.empty() || line.second != -1) {
        if (line.second != -1) {
            lines.push_back(line);
        }
    }
    return lines;
}

} // namespace IO_Helper