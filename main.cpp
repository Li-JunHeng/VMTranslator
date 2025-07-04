#include <iostream>
#include <string>
#include "Helper/Usage.h"
#include "IO_Helper/Helper.h"
#include "Command/Parser.h"
#include "Command/CodeWriter.h"

using namespace std;

int main(int argc, char* argv[]) {
    Helper::Usage cli(argc, argv);
    if (cli.isVersion()) Helper::Usage::printVersion();
    if (cli.isHelp()) Helper::Usage::printHelp();
    if (cli.isHelp() || cli.isVersion()) return 0;

    string inputPath = cli.getInputPath();
    string outputPath = cli.getOutputPath();

    IO_Helper::Helper helper(inputPath, outputPath);
    helper.translate();

    return 0;
}