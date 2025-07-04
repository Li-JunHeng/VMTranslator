#pragma once

#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <string>
#include <fstream>
#include <vector>

namespace IO_Helper {

class InputHelper {
private:
    std::ifstream fileStream;
    std::istream* inputStream;
    bool ownsStream;
    int lineNumber;

public:
    InputHelper(const std::string& filename);
    InputHelper(std::istream& stream);
    InputHelper(const InputHelper& other);
    ~InputHelper();

    bool isValid() const;
    std::pair<std::string, int> readLine();
    std::vector<std::pair<std::string, int>> readAllLines();
    int getCurrentLineNumber() const { return lineNumber; }
};

} // namespace IO_Helper

#endif // INPUTHELPER_H