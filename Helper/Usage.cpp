#include "Usage.h"
#include <iostream>
#include <filesystem>
#include <string>
#include "../Logger/ErrorLogger.h"

using namespace std;
namespace Helper {
namespace fs = filesystem;

Usage::Usage(int argc, char* argv[]) : helpFlag(false), versionFlag(false) {
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    parseArgs(argc, argv);
}

void Usage::parseArgs(int argc, char* argv[]) {
    if (argc == 1) {
        printHelp();
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            helpFlag = true;
            return;
        } else if (arg == "--version" || arg == "-v") {
            versionFlag = true;
            return;
        } else if (inputPath.empty()) {
            inputPath = arg;
            // 验证输入路径
            if (!isVMFile(inputPath) && !isDirectory(inputPath)) {
                Logger::ErrorLogger::IOError(runtime_error("输入路径不是有效的VM文件或目录"), -1, inputPath);
                exit(1);
            }
        } else if (outputPath.empty()) {
            outputPath = arg;
        } else {
            Logger::ErrorLogger::IOError(runtime_error("无效的参数: " + arg), -1, "");
            printHelp();
            exit(1);
        }
    }

    if (inputPath.empty()) {
        Logger::ErrorLogger::IOError(runtime_error("未提供输入路径"), -1, "");
        printHelp();
        exit(1);
    }
}

bool Usage::isVMFile(const string& path) const {
    return fs::is_regular_file(path) && fs::path(path).extension() == ".vm";
}

bool Usage::isDirectory(const string& path) const {
    return fs::is_directory(path);
}

void Usage::printHelp() {
    cout << "VMTranslator - 将VM语言转换为Hack汇编语言\n";
    cout << "用法: VMTranslator [选项] <输入路径> [输出路径]\n\n";
    cout << "选项:\n";
    cout << "  -h, --help      显示此帮助信息并退出\n";
    cout << "  -v, --version   显示版本信息并退出\n\n";
    cout << "输入路径:\n";
    cout << "  可以是单个.vm文件或包含.vm文件的目录\n\n";
    cout << "输出路径:\n";
    cout << "  指定输出.asm文件的路径，默认为:\n";
    cout << "  - 若输入为文件: 与输入文件同目录，使用相同文件名但扩展名为.asm\n";
    cout << "  - 若输入为目录: 在该目录下创建与目录同名的.asm文件\n\n";
    cout << "示例:\n";
    cout << "  VMTranslator test.vm\n";
    cout << "  VMTranslator ./project_dir\n";
    cout << "  VMTranslator input.vm output.asm\n";
}

void Usage::printVersion() {
    cout << "VMTranslator v1.0.0\n";
    cout << "Copyright (c) 2023\n";
}

} // namespace Helper