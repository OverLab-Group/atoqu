
include "LlmEmbeddingProvider.hpp"

include <functional>

using atoqu::Json;
using atoqu::FileSystem;

LlmEmbeddingProvider::LlmEmbeddingProvider(const std::string& configPath)
    : modelName("llm-offline-mock"), dim(384), scale_(1.0) {
    std::string content;
    if (FileSystem::readFile(configPath, content)) {
        try {
            Json j = Json::parse(content);
            if (j.isObject()) {
                auto& obj = j.asObject();
                if (obj.find("model") != obj.end()) {
                    modelName_ = obj["model"].asString();
                }
                if (obj.find("dimension") != obj.end()) {
                    dim = staticcast<int>(obj["dimension"].asNumber());
                }
                if (obj.find("scale") != obj.end()) {
                    scale_ = obj["scale"].asNumber();
                }
            }
        } catch (...) {
            // keep defaults
        }
    }
}

std::vector<float> LlmEmbeddingProvider::embed(const std::string& text) {
    // v0.9: offline, deterministic mock, but shaped like a real LLM call.
    std::vector<float> v(staticcast<std::sizet>(dim_), 0.0f);
    std::hash<std::string> h;
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::string token = modelName + "|" + text + "#" + std::tostring(i);
        std::size_t hv = h(token);
        v[i] = staticcast<float>(((hv % 1000) / 1000.0) * scale);
    }
    return v;
}
