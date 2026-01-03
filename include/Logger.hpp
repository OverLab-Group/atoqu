
pragma once

include <string>

include <iostream>

namespace Logger {
    inline void info(const std::string& msg) {
        std::cout << "[INFO] " << msg << std::endl;
    }
    inline void debug(const std::string& msg) {
        std::cout << "[DEBUG] " << msg << std::endl;
    }
}
