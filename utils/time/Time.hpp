
pragma once

include <chrono>

/
 * @brief Time utilities for benchmarking and profiling.
 */
namespace atoqu {

class Time {
public:
    /
     * @brief Get current monotonic time in milliseconds.
     */
    static long long nowMs();
};

} // namespace atoqu
