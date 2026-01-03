
include "Config.hpp"

include <nlohmann/json.hpp>

include <fstream>

using json = nlohmann::json;

std::vector<ModeConfig> Config::loadModes(const std::string& path) {
    std::vector<ModeConfig> out;
    std::ifstream in(path);
    if (!in) return out;
    json j;
    in >> j;
    if (!j.contains("modes")) return out;
    for (auto& m : j["modes"]) {
        ModeConfig mc;
        mc.name    = m.value("name", "");
        mc.enabled = m.value("enabled", true);
        mc.weight  = m.value("weight", 1.0);
        if (!mc.name.empty()) out.push_back(mc);
    }
    return out;
}

LlmConfig Config::loadLlm(const std::string& path) {
    LlmConfig cfg;
    std::ifstream in(path);
    if (!in) {
        cfg.provider = "hash";
        cfg.embeddingSize = 16;
        return cfg;
    }
    json j;
    in >> j;
    cfg.provider      = j.value("provider", "hash");
    cfg.embeddingSize = j.value("embeddingSize", 16);
    cfg.endpoint      = j.value("endpoint", "");
    cfg.apiKey        = j.value("apiKey", "");
    return cfg;
}
