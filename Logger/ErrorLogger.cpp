#include "ErrorLogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;
namespace Logger {

void ErrorLogger::IOError(const runtime_error& e, int lineNumber, const string& path) {
    cerr << "[ERROR] I/O Error: " << e.what() << endl;
    if (!path.empty()) {
        cerr << "  File: " << path << endl;
    }
    if (lineNumber != -1) {
        cerr << "  Line: " << lineNumber << endl;
    }
}

void ErrorLogger::cmdLogError(const invalid_argument& e, int lineNumber, const string& command) {
    cerr << "[ERROR] Command Parsing Error: " << e.what() << endl;
    if (lineNumber != -1) {
        cerr << "  Line: " << lineNumber << endl;
    }
    if (!command.empty()) {
        cerr << "  Command: \"" << command << "\"" << endl;
    }
}

void ErrorLogger::transLogError(const exception& e, const string& context) {
    cerr << "[ERROR] Translation Error: " << e.what() << endl;
    if (!context.empty()) {
        cerr << "  Context: " << context << endl;
    }
}

void ErrorLogger::logError(const string& message) {
    time_t now = time(nullptr);
    cerr << "[ERROR] [" << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << "] " << message << endl;
}

} // namespace Logger