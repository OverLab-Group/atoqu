
pragma once

include <string>

include <vector>

struct ModeConfig {
    std::string name;
    bool enabled;
    double weight;
};

struct LlmConfig {
    std::string provider;
    std::size_t embeddingSize;
    std::string endpoint;
    std::string apiKey;
};

class Config {
public:
    static std::vector<ModeConfig> loadModes(const std::string& path);
    static LlmConfig loadLlm(const std::string& path);
};
