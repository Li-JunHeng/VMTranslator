#include "CodeWriter.h"
#include <iostream>
#include <algorithm>
#include "../Logger/ErrorLogger.h"

using namespace std;

namespace Command {

CodeWriter::CodeWriter(const string& filename) : labelCounter(0) {
    outputStream.open(filename);
    if (!outputStream.is_open()) {
        Logger::ErrorLogger::IOError(runtime_error("无法打开输出文件"), -1, filename);
    }

    // 初始化段映射
    segmentMap = {
        {"local", "LCL"},
        {"argument", "ARG"},
        {"this", "THIS"},
        {"that", "THAT"},
        {"pointer", "3"},
        {"temp", "5"}
    };
}

CodeWriter::~CodeWriter() {
    close();
}

void CodeWriter::setFileName(const string& filename) {
    currentFileName = filename;
    // 提取文件名（不含路径和扩展名）
    size_t lastSlash = filename.find_last_of("/\"");
    if (lastSlash != string::npos) {
        currentFileName = filename.substr(lastSlash + 1);
    }
    size_t lastDot = currentFileName.find_last_of('.');
    if (lastDot != string::npos) {
        currentFileName = currentFileName.substr(0, lastDot);
    }
}

string CodeWriter::generateLabel(const string& base) {
    return base + "$" + to_string(labelCounter++);
}

void CodeWriter::writeArithmeticAdd() {
    outputStream << "// add\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "M=D+M\n";
}

void CodeWriter::writeArithmeticSub() {
    outputStream << "// sub\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "M=M-D\n";
}

void CodeWriter::writeArithmeticNeg() {
    outputStream << "// neg\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=-M\n";
}

void CodeWriter::writeArithmeticEq() {
    string labelTrue = generateLabel("TRUE");
    string labelEnd = generateLabel("END");
    outputStream << "// eq\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "D=M-D\n";
    outputStream << "@" << labelTrue << "\n";
    outputStream << "D;JEQ\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=0\n";
    outputStream << "@" << labelEnd << "\n";
    outputStream << "0;JMP\n";
    outputStream << "(" << labelTrue << ")\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=-1\n";
    outputStream << "(" << labelEnd << ")\n";
}

void CodeWriter::writeArithmeticGt() {
    string labelTrue = generateLabel("TRUE");
    string labelEnd = generateLabel("END");
    outputStream << "// gt\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "D=M-D\n";
    outputStream << "@" << labelTrue << "\n";
    outputStream << "D;JGT\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=0\n";
    outputStream << "@" << labelEnd << "\n";
    outputStream << "0;JMP\n";
    outputStream << "(" << labelTrue << ")\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=-1\n";
    outputStream << "(" << labelEnd << ")\n";
}

void CodeWriter::writeArithmeticLt() {
    string labelTrue = generateLabel("TRUE");
    string labelEnd = generateLabel("END");
    outputStream << "// lt\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "D=M-D\n";
    outputStream << "@" << labelTrue << "\n";
    outputStream << "D;JLT\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=0\n";
    outputStream << "@" << labelEnd << "\n";
    outputStream << "0;JMP\n";
    outputStream << "(" << labelTrue << ")\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=-1\n";
    outputStream << "(" << labelEnd << ")\n";
}

void CodeWriter::writeArithmeticAnd() {
    outputStream << "// and\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "M=D&M\n";
}

void CodeWriter::writeArithmeticOr() {
    outputStream << "// or\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "A=A-1\n";
    outputStream << "M=D|M\n";
}

void CodeWriter::writeArithmeticNot() {
    outputStream << "// not\n";
    outputStream << "@SP\n";
    outputStream << "A=M-1\n";
    outputStream << "M=!M\n";
}

void CodeWriter::writeArithmetic(const string& command) {
    outputStream << "// ----------------------------\n";
    outputStream << "// " << command << "\n";
    outputStream << "// ----------------------------\n";

    if (command == "add") writeArithmeticAdd();
    else if (command == "sub") writeArithmeticSub();
    else if (command == "neg") writeArithmeticNeg();
    else if (command == "eq") writeArithmeticEq();
    else if (command == "gt") writeArithmeticGt();
    else if (command == "lt") writeArithmeticLt();
    else if (command == "and") writeArithmeticAnd();
    else if (command == "or") writeArithmeticOr();
    else if (command == "not") writeArithmeticNot();
    else {
        Logger::ErrorLogger::transLogError(invalid_argument("未知算术命令: " + command), "CodeWriter::writeArithmetic");
    }
}

void CodeWriter::writePushPop(CommandType commandType, const string& segment, int index) {
    outputStream << "// ----------------------------\n";
    outputStream << "// " << (commandType == CommandType::C_PUSH ? "push" : "pop") << " " << segment << " " << index << "\n";
    outputStream << "// ----------------------------\n";

    if (commandType == CommandType::C_PUSH) {
        if (segment == "constant") {
            outputStream << "@" << index << "\n";
            outputStream << "D=A\n";
            outputStream << "@SP\n";
            outputStream << "M=M+1\n";
            outputStream << "A=M-1\n";
            outputStream << "M=D\n";
        } else if (segment == "static") {
            outputStream << "@" << currentFileName << "." << index << "\n";
            outputStream << "D=M\n";
            outputStream << "@SP\n";
            outputStream << "M=M+1\n";
            outputStream << "A=M-1\n";
            outputStream << "M=D\n";
        } else if (segmentMap.find(segment) != segmentMap.end()) {
            if (segment == "pointer" || segment == "temp") {
                outputStream << "@" << segmentMap[segment] << "\n";
                outputStream << "D=A\n";
            } else {
                outputStream << "@" << segmentMap[segment] << "\n";
                outputStream << "D=M\n";
            }
            outputStream << "@" << index << "\n";
            outputStream << "A=D+A\n";
            outputStream << "D=M\n";
            outputStream << "@SP\n";
            outputStream << "M=M+1\n";
            outputStream << "A=M-1\n";
            outputStream << "M=D\n";
        } else {
            Logger::ErrorLogger::transLogError(invalid_argument("未知段: " + segment), "CodeWriter::writePushPop");
        }
    } else { // C_POP
        if (segment == "static") {
            outputStream << "@SP\n";
            outputStream << "AM=M-1\n";
            outputStream << "D=M\n";
            outputStream << "@" << currentFileName << "." << index << "\n";
            outputStream << "M=D\n";
        } else if (segmentMap.find(segment) != segmentMap.end()) {
            outputStream << "@" << segmentMap[segment] << "\n";
            if (segment == "pointer" || segment == "temp") {
                outputStream << "D=A\n";
            } else {
                outputStream << "D=M\n";
            }
            outputStream << "@" << index << "\n";
            outputStream << "D=D+A\n";
            outputStream << "@R13\n";
            outputStream << "M=D\n";
            outputStream << "@SP\n";
            outputStream << "AM=M-1\n";
            outputStream << "D=M\n";
            outputStream << "@R13\n";
            outputStream << "A=M\n";
            outputStream << "M=D\n";
        } else {
            Logger::ErrorLogger::transLogError(invalid_argument("未知段: " + segment), "CodeWriter::writePushPop");
        }
    }
}

void CodeWriter::writeLabel(const string& label) {
    outputStream << "// label " << label << "\n";
    outputStream << "(" << currentFunction << "$" << label << ")\n";
}

void CodeWriter::writeGoto(const string& label) {
    outputStream << "// goto " << label << "\n";
    outputStream << "@" << currentFunction << "$" << label << "\n";
    outputStream << "0;JMP\n";
}

void CodeWriter::writeIf(const string& label) {
    outputStream << "// if-goto " << label << "\n";
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "@" << currentFunction << "$" << label << "\n";
    outputStream << "D;JNE\n";
}

void CodeWriter::writeFunction(const string& functionName, int nVars) {
    currentFunction = functionName;
    outputStream << "// function " << functionName << " " << nVars << "\n";
    outputStream << "(" << functionName << ")\n";

    // 初始化nVars个局部变量为0
    for (int i = 0; i < nVars; i++) {
        outputStream << "@0\n";
        outputStream << "D=A\n";
        outputStream << "@SP\n";
        outputStream << "M=M+1\n";
        outputStream << "A=M-1\n";
        outputStream << "M=D\n";
    }
}

void CodeWriter::writeCall(const string& functionName, int nArgs) {
    string returnLabel = generateLabel(functionName + "$ret");

    outputStream << "// call " << functionName << " " << nArgs << "\n";
    // 保存返回地址
    outputStream << "@" << returnLabel << "\n";
    outputStream << "D=A\n";
    outputStream << "@SP\n";
    outputStream << "M=M+1\n";
    outputStream << "A=M-1\n";
    outputStream << "M=D\n";

    // 保存LCL、ARG、THIS、THAT
    for (const string& reg : {"LCL", "ARG", "THIS", "THAT"}) {
        outputStream << "@" << reg << "\n";
        outputStream << "D=M\n";
        outputStream << "@SP\n";
        outputStream << "M=M+1\n";
        outputStream << "A=M-1\n";
        outputStream << "M=D\n";
    }

    // 重新定位ARG
    outputStream << "@SP\n";
    outputStream << "D=M\n";
    outputStream << "@5\n";
    outputStream << "D=D-A\n";
    outputStream << "@" << nArgs << "\n";
    outputStream << "D=D-A\n";
    outputStream << "@ARG\n";
    outputStream << "M=D\n";

    // 重新定位LCL
    outputStream << "@SP\n";
    outputStream << "D=M\n";
    outputStream << "@LCL\n";
    outputStream << "M=D\n";

    // 跳转到函数
    outputStream << "@" << functionName << "\n";
    outputStream << "0;JMP\n";

    // 定义返回标签
    outputStream << "(" << returnLabel << ")\n";
}

void CodeWriter::writeReturn() {
    outputStream << "// return\n";
    // 保存LCL到R13
    outputStream << "@LCL\n";
    outputStream << "D=M\n";
    outputStream << "@R13\n";
    outputStream << "M=D\n";

    // 保存返回地址到R14
    outputStream << "@5\n";
    outputStream << "A=D-A\n";
    outputStream << "D=M\n";
    outputStream << "@R14\n";
    outputStream << "M=D\n";

    // 将返回值放到ARG[0]
    outputStream << "@SP\n";
    outputStream << "AM=M-1\n";
    outputStream << "D=M\n";
    outputStream << "@ARG\n";
    outputStream << "A=M\n";
    outputStream << "M=D\n";

    // 重置SP
    outputStream << "D=A+1\n";
    outputStream << "@SP\n";
    outputStream << "M=D\n";

    // 恢复THAT、THIS、ARG、LCL
    for (const pair<string, int>& reg : {make_pair("THAT", 1), make_pair("THIS", 2), make_pair("ARG", 3), make_pair("LCL", 4)}) {
        outputStream << "@R13\n";
        outputStream << "D=M\n";
        outputStream << "@" << reg.second << "\n";
        outputStream << "A=D-A\n";
        outputStream << "D=M\n";
        outputStream << "@" << reg.first << "\n";
        outputStream << "M=D\n";
    }

    // 跳转到返回地址
    outputStream << "@R14\n";
    outputStream << "A=M\n";
    outputStream << "0;JMP\n";
}

void CodeWriter::close() {
    if (outputStream.is_open()) {
        outputStream.close();
    }
}

} // namespace Command