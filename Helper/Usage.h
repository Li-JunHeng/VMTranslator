#pragma once

#ifndef USAGE_H
#define USAGE_H

#include <string>
#include <vector>

namespace Helper {

class Usage {
private:
    std::string inputPath;
    std::string outputPath;
    bool helpFlag;
    bool versionFlag;
    std::vector<std::string> args;

    void parseArgs(int argc, char* argv[]);
    bool isVMFile(const std::string& path) const;
    bool isDirectory(const std::string& path) const;

public:
    Usage(int argc, char* argv[]);
    ~Usage() = default;

    bool isHelp() const { return helpFlag; }
    bool isVersion() const { return versionFlag; }
    std::string getInputPath() const { return inputPath; }
    std::string getOutputPath() const { return outputPath; }

    static void printHelp();
    static void printVersion();
};

} // namespace Helper

#endif // USAGE_H