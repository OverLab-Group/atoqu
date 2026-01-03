
pragma once

include <string>

/
 * @brief Minimal filesystem utilities for Atoqu Engine.
 */
namespace atoqu {

class FileSystem {
public:
    /
     * @brief Read entire file into a string.
     * @return true on success, false on failure.
     */
    static bool readFile(const std::string& path, std::string& out);

    /
     * @brief Write string data to file (overwrite).
     * @return true on success, false on failure.
     */
    static bool writeFile(const std::string& path, const std::string& data);

    /
     * @brief Check if file exists.
     */
    static bool exists(const std::string& path);
};

} // namespace atoqu
