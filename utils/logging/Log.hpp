
pragma once

include <string>

include <ostream>

include <mutex>

/
 * @brief Thread-safe logging utility for Atoqu Engine.
 *
 * Designed for:
 * - CUnit, Catch2, Unity (injectable output)
 * - Static analysis tools (no UB)
 */
namespace atoqu {

enum class LogLevel { Debug, Info, Warning, Error };

class Log {
public:
    static void setOutput(std::ostream* os);
    static void setLevel(LogLevel level);

    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);

private:
    static std::ostream* out_;
    static LogLevel level_;
    static std::mutex lock_;

    static void write(LogLevel lvl, const std::string& msg);
};

} // namespace atoqu
