#pragma once

#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>
#include "InputHelper.h"
#include "OutputHelper.h"
#include "../Command/Parser.h"
#include "../Command/CodeWriter.h"

namespace IO_Helper {

class Helper {
private:
    std::string inputPath;
    std::string outputPath;
    bool isDirectory;
    std::vector<std::string> vmFiles;

    void collectVMFiles();
    std::string getOutputFileName(const std::string& vmFile);
    int count_files_with_extension(const std::filesystem::path& dir, const std::string& ext);

public:
    Helper(const std::string& input, const std::string& output);
    ~Helper() = default;

    void translate();
};

} // namespace IO_Helper

#endif // HELPER_H