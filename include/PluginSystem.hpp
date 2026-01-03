
pragma once

include <string>

include <vector>

/
 * @brief Simple plugin discovery system.
 *
 * Each plugin is a directory under plugins/ with a manifest file.
 */
struct PluginInfo {
    std::string name;
    std::string path;
    std::string description;
};

class PluginSystem {
public:
    explicit PluginSystem(const std::string& rootDir);

    void scan();

    std::vector<PluginInfo> list() const;

private:
    std::string rootDir_;
    std::vector<PluginInfo> plugins_;
};
