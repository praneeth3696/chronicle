#pragma once
#include <string>
#include <vector>

class InputBuffer {
public:
    // Trims leading and trailing whitespace from a string
    static std::string trim(const std::string& str);

    // Writes clean command history to log file silently
    void processAndLog(const std::vector<std::string>& history, const std::string& filename);
};
