#include "InputHelper.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include "../Logger/ErrorLogger.h"

using namespace std;
namespace IO_Helper {

// 辅助函数：修剪字符串首尾空白字符
static string trim(const string& s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

InputHelper::InputHelper(const string& filename) : inputStream(&fileStream), ownsStream(true), lineNumber(0) {
    fileStream.open(filename);
    if (!fileStream.is_open()) {
        Logger::ErrorLogger::IOError(runtime_error("无法打开输入文件"), -1, filename);
        inputStream = nullptr;
    }
}

InputHelper::InputHelper(istream& stream) : inputStream(&stream), ownsStream(false), lineNumber(0) {
    if (!stream.good()) {
        Logger::ErrorLogger::IOError(runtime_error("输入流状态无效"), -1, "");
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

pair<string, int> InputHelper::readLine() {
    if (!isValid()) {
        return {"", -1};
    }

    string line;
    if (getline(*inputStream, line)) {
        lineNumber++;
        // 移除行内注释
        size_t commentPos = line.find("//");
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }
        // 修剪空白字符
        string trimmedLine = trim(line);
        return {trimmedLine, lineNumber};
    }

    // 到达文件末尾
    return {"", -1};
}

vector<pair<string, int>> InputHelper::readAllLines() {
    vector<pair<string, int>> lines;
    pair<string, int> line;
    while (!(line = readLine()).first.empty() || line.second != -1) {
        if (line.second != -1) {
            lines.push_back(line);
        }
    }
    return lines;
}

} // namespace IO_Helper