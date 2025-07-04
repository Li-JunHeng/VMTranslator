#include "Parser.h"
#include <sstream>
#include <algorithm>
#include "../Logger/ErrorLogger.h"

using namespace std;
namespace Command {

Parser::Parser(const string& filename) : lineNumber(0) {
    inputStream.open(filename);
    if (!inputStream.is_open()) {
        Logger::ErrorLogger::IOError(runtime_error("无法打开文件"), -1, filename);
    }
}

Parser::~Parser() {
    if (inputStream.is_open()) {
        inputStream.close();
    }
}

bool Parser::hasMoreCommands() {
    return inputStream.peek() != EOF;
}

void Parser::advance() {
    string line;
    while (getline(inputStream, line)) {
        lineNumber++;
        // 移除行内注释
        size_t commentPos = line.find("//");
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }
        // 移除首尾空白
        size_t start = line.find_first_not_of(" 	");
        if (start == string::npos) continue; // 空行
        size_t end = line.find_last_not_of(" 	");
        currentCommand = line.substr(start, end - start + 1);
        tokenize();
        return;
    }
    currentCommand = "";
    tokens.clear();
}

void Parser::tokenize() {
    tokens.clear();
    istringstream iss(currentCommand);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
}

bool Parser::nextCommand() {
    if (!hasMoreCommands()) return false;
    advance();
    return !currentCommand.empty();
}

CommandType Parser::commandType() const {
    if (tokens.empty()) {
        return static_cast<CommandType>(-1);
    }

    const string& cmd = tokens[0];
    if (cmd == "add" || cmd == "sub" || cmd == "neg" ||
        cmd == "eq" || cmd == "gt" || cmd == "lt" ||
        cmd == "and" || cmd == "or" || cmd == "not") {
        return CommandType::C_ARITHMETIC;
    } else if (cmd == "push") {
        return CommandType::C_PUSH;
    } else if (cmd == "pop") {
        return CommandType::C_POP;
    } else if (cmd == "label") {
        return CommandType::C_LABEL;
    } else if (cmd == "goto") {
        return CommandType::C_GOTO;
    } else if (cmd == "if-goto") {
        return CommandType::C_IF;
    } else if (cmd == "function") {
        return CommandType::C_FUNCTION;
    } else if (cmd == "return") {
        return CommandType::C_RETURN;
    } else if (cmd == "call") {
        return CommandType::C_CALL;
    } else {
        Logger::ErrorLogger::cmdLogError(invalid_argument("未知命令类型: " + cmd), lineNumber, currentCommand);
        return static_cast<CommandType>(-1);
    }
}

string Parser::arg1() const {
    CommandType type = commandType();
    if (type == CommandType::C_ARITHMETIC) {
        return tokens[0];
    } else if (type != CommandType::C_RETURN && tokens.size() > 1) {
        return tokens[1];
    }
    Logger::ErrorLogger::cmdLogError(invalid_argument("无效的arg1访问"), lineNumber, currentCommand);
    return "";
}

int Parser::arg2() const {
    CommandType type = commandType();
    if ((type == CommandType::C_PUSH || type == CommandType::C_POP ||
         type == CommandType::C_FUNCTION || type == CommandType::C_CALL) &&
        tokens.size() > 2) {
        try {
            return stoi(tokens[2]);
        } catch (const exception& e) {
            Logger::ErrorLogger::cmdLogError(invalid_argument("arg2不是整数: " + tokens[2]), lineNumber, currentCommand);
        }
    }
    Logger::ErrorLogger::cmdLogError(invalid_argument("无效的arg2访问"), lineNumber, currentCommand);
    return -1;
}

int Parser::getLineNumber() const {
    return lineNumber;
}

} // namespace Command