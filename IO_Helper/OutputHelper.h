#pragma once

#ifndef OUTPUTHELPER_H
#define OUTPUTHELPER_H

#include <string>
#include <fstream>
#include <vector>

namespace IO_Helper {

class OutputHelper {
private:
    std::ofstream fileStream;
    std::ostream* outputStream;
    bool ownsStream;

public:
    OutputHelper(const std::string& filename);
    OutputHelper(std::ostream& stream);
    ~OutputHelper();

    bool isValid() const;
    void writeLine(const std::string& line);
    void writeLines(const std::vector<std::string>& lines);
    void close();
};

} // namespace IO_Helper

#endif // OUTPUTHELPER_H