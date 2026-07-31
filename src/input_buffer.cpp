    #include "../include/input_buffer.hpp"
    #include <fstream>
    #include <iostream>
    #include <chrono>   // Added for system clock
    #include <iomanip>  // Added for put_time
    #include <sstream>

    std::string InputBuffer::trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return ""; // String is all whitespace
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    void InputBuffer::processAndLog(const std::vector<std::string>& history, const std::string& filename) {
        std::ofstream logFile(filename, std::ios::app); // Append mode

        if (!logFile.is_open()) {
            std::cerr << "[Error] InputBuffer could not open log file: " << filename << std::endl;
            return;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S") << "]";

            logFile << "\n--- Session Run: " << ss.str() << " ---\n";


        for (const std::string& rawCmd : history) {
            std::string cleanCmd = trim(rawCmd);
            
            // Skip logging if the command became empty after trimming
            if (!cleanCmd.empty()) {
                logFile << cleanCmd << "\n";
            }
        }

        logFile.close();
        std::cout << "[InputBuffer] Clean logs successfully written to " << filename << std::endl;

    }