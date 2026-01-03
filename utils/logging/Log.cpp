
include "Log.hpp"

include <iostream>

namespace atoqu {

std::ostream* Log::out_ = &std::cout;
LogLevel Log::level_ = LogLevel::Debug;
std::mutex Log::lock_;

void Log::setOutput(std::ostream* os) {
    std::lockguard<std::mutex> g(lock);
    out_ = os;
}

void Log::setLevel(LogLevel level) {
    std::lockguard<std::mutex> g(lock);
    level_ = level;
}

void Log::write(LogLevel lvl, const std::string& msg) {
    std::lockguard<std::mutex> g(lock);
    if (lvl < level_) return;

    switch (lvl) {
        case LogLevel::Debug:   (*out_) << "[DEBUG] "; break;
        case LogLevel::Info:    (*out_) << "[INFO] "; break;
        case LogLevel::Warning: (*out_) << "[WARN] "; break;
        case LogLevel::Error:   (*out_) << "[ERROR] "; break;
    }
    (*out_) << msg << "\n";
}

void Log::debug(const std::string& msg) { write(LogLevel::Debug, msg); }
void Log::info (const std::string& msg) { write(LogLevel::Info,  msg); }
void Log::warn (const std::string& msg) { write(LogLevel::Warning, msg); }
void Log::error(const std::string& msg) { write(LogLevel::Error, msg); }

} // namespace atoqu
