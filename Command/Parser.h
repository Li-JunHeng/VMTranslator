#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <vector>

namespace Command {

enum class CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

class Parser {
private:
    std::ifstream inputStream;
    std::string currentCommand;
    std::vector<std::string> tokens;
    int lineNumber;

    bool hasMoreCommands();
    void advance();
    void tokenize();

public:
    Parser(const std::string& filename);
    ~Parser();

    bool nextCommand();
    CommandType commandType() const;
    std::string arg1() const;
    int arg2() const;
    int getLineNumber() const;
};

} // namespace Command

#endif // PARSER_H