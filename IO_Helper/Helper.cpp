#include "Helper.h"
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "../Logger/ErrorLogger.h"

namespace IO_Helper {
namespace fs = std::filesystem;

Helper::Helper(const std::string& input, const std::string& output) : inputPath(input), outputPath(output) {
    // 检查输入路径是否为目录
    if (fs::is_directory(inputPath)) {
        isDirectory = true;
        collectVMFiles();
        if (vmFiles.empty()) {
            Logger::ErrorLogger::IOError(std::runtime_error("目录中未找到VM文件"), -1, inputPath);
        }
        // 如果未指定输出路径，使用目录名作为输出文件名
        if (outputPath.empty()) {
            outputPath = inputPath + "/" + fs::path(inputPath).filename().string() + ".asm";
        }
    } else {
        isDirectory = false;
        // 检查输入文件是否为.vm文件
        if (fs::path(inputPath).extension() != ".vm") {
            Logger::ErrorLogger::IOError(std::runtime_error("输入文件不是VM文件"), -1, inputPath);
        }
        vmFiles.push_back(inputPath);
        // 如果未指定输出路径，使用输入文件名替换扩展名为.asm
        if (outputPath.empty()) {
            outputPath = fs::path(inputPath).replace_extension(".asm").string();
        }
    }
}

void Helper::collectVMFiles() {
    try {
        for (const auto& entry : fs::directory_iterator(inputPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".vm") {
                vmFiles.push_back(entry.path().string());
            }
        }
        // 按文件名排序
        std::sort(vmFiles.begin(), vmFiles.end());
    } catch (const fs::filesystem_error& e) {
        Logger::ErrorLogger::IOError(std::runtime_error("目录访问错误: " + std::string(e.what())), -1, inputPath);
    }
}

std::string Helper::getOutputFileName(const std::string& vmFile) {
    return fs::path(vmFile).filename().replace_extension(".asm").string();
}

void Helper::translate() {
    Command::CodeWriter codeWriter(outputPath);

    // 如果是目录，添加引导代码
    if (isDirectory) {
        codeWriter.writeCall("Sys.init", 0);
    }

    for (const std::string& vmFile : vmFiles) {
        Command::Parser parser(vmFile);
        if (!parser.nextCommand()) {
            Logger::ErrorLogger::IOError(std::runtime_error("无法读取VM文件"), -1, vmFile);
            continue;
        }

        codeWriter.setFileName(vmFile);
        std::cout << "正在处理: " << vmFile << std::endl;

        do {
            Command::CommandType type = parser.commandType();
            std::string arg1 = parser.arg1();
            int arg2 = parser.arg2();

            switch (type) {
                case Command::CommandType::C_ARITHMETIC:
                    codeWriter.writeArithmetic(arg1);
                    break;
                case Command::CommandType::C_PUSH:
                case Command::CommandType::C_POP:
                    codeWriter.writePushPop(type, arg1, arg2);
                    break;
                case Command::CommandType::C_LABEL:
                    codeWriter.writeLabel(arg1);
                    break;
                case Command::CommandType::C_GOTO:
                    codeWriter.writeGoto(arg1);
                    break;
                case Command::CommandType::C_IF:
                    codeWriter.writeIf(arg1);
                    break;
                case Command::CommandType::C_FUNCTION:
                    codeWriter.writeFunction(arg1, arg2);
                    break;
                case Command::CommandType::C_CALL:
                    codeWriter.writeCall(arg1, arg2);
                    break;
                case Command::CommandType::C_RETURN:
                    codeWriter.writeReturn();
                    break;
                default:
                    Logger::ErrorLogger::cmdLogError(std::invalid_argument("未知命令类型"), parser.getLineNumber(), "");
            }
        } while (parser.nextCommand());
    }

    codeWriter.close();
    std::cout << "翻译完成，输出文件: " << outputPath << std::endl;
}

} // namespace IO_Helper