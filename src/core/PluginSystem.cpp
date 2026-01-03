
include "PluginSystem.hpp"

include "FileSystem.hpp"

include "Json.hpp"

include <filesystem>

using atoqu::FileSystem;
using atoqu::Json;
namespace fs = std::filesystem;

PluginSystem::PluginSystem(const std::string& rootDir)
    : rootDir_(rootDir) {}

void PluginSystem::scan() {
    plugins_.clear();
    if (!fs::exists(rootDir_)) return;

    for (auto& entry : fs::directoryiterator(rootDir)) {
        if (!entry.is_directory()) continue;
        auto manifestPath = entry.path() / "plugin.json";
        if (!FileSystem::exists(manifestPath.string())) continue;

        std::string content;
        if (!FileSystem::readFile(manifestPath.string(), content)) continue;

        try {
            Json j = Json::parse(content);
            if (!j.isObject()) continue;
            auto& obj = j.asObject();
            PluginInfo info;
            info.name = obj["name"].asString();
            info.description = obj["description"].asString();
            info.path = entry.path().string();
            plugins.pushback(info);
        } catch (...) {
            continue;
        }
    }
}

std::vector<PluginInfo> PluginSystem::list() const {
    return plugins_;
}
