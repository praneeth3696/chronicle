#pragma once
#include <string>
#include <vector>

class InputBuffer {
private:
    // Helper function to trim whitespaces from a string
    std::string trim(const std::string& str);

public:
    // Takes the history array, trims each element, and writes to the log file
    void processAndLog(const std::vector<std::string>& history, const std::string& filename);
};
