#pragma once

#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <string>
#include <fstream>
#include <unordered_map>
#include "Parser.h"

namespace Command {

class CodeWriter {
private:
    std::ofstream outputStream;
    std::string currentFileName;
    std::string currentFunction;
    int labelCounter;
    std::unordered_map<std::string, std::string> segmentMap;

    std::string generateLabel(const std::string& base);
    void writeArithmeticAdd();
    void writeArithmeticSub();
    void writeArithmeticNeg();
    void writeArithmeticEq();
    void writeArithmeticGt();
    void writeArithmeticLt();
    void writeArithmeticAnd();
    void writeArithmeticOr();
    void writeArithmeticNot();

public:
    CodeWriter(const std::string& filename);
    ~CodeWriter();

    void setFileName(const std::string& filename);
    void writeArithmetic(const std::string& command);
    void writePushPop(CommandType commandType, const std::string& segment, int index);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeFunction(const std::string& functionName, int nVars);
    void writeCall(const std::string& functionName, int nArgs);
    void writeReturn();
    void close();
};

} // namespace Command

#endif // CODEWRITER_H