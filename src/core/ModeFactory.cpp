
include "ModeFactory.hpp"

include "LiteralMode.hpp"

include "VectorMode.hpp"

include "HybridMode.hpp"

include "NormalMode.hpp"

include "BM25Mode.hpp"

include "RecencyMode.hpp"

include "TagBoostMode.hpp"

include "HashEmbeddingProvider.hpp"

include "LlmEmbeddingProvider.hpp"

include "Json.hpp"

include "FileSystem.hpp"

include "atoqu/gpu/GpuBackend.hpp"

include "GpuBackend.hpp"

include "Log.hpp"

using atoqu::Json;
using atoqu::FileSystem;
using atoqu::Log;
using atoqu::LogLevel;

static std::unique_ptr<EmbeddingProvider> createEmbeddingProvider() {
    std::string content;
    if (FileSystem::readFile("config/llm.json", content)) {
        try {
            Json j = Json::parse(content);
            if (j.isObject()) {
                auto& obj = j.asObject();
                if (obj.find("provider") != obj.end()) {
                    std::string provider = obj["provider"].asString();
                    if (provider == "llm") {
                        return std::make_unique<LlmEmbeddingProvider>("config/llm.json");
                    }
                }
            }
        } catch (...) {
            // fall through
        }
    }
    return std::make_unique<HashEmbeddingProvider>(16);
}

std::vector<std::unique_ptr<SearchMode>>
ModeFactory::createModes(const std::vector<ModeConfig>& configs,
                         DocumentStore& docs,
                         VectorStore& vecs) {
    std::vector<std::unique_ptr<SearchMode>> modes;
    auto embedder = createEmbeddingProvider();
    EmbeddingProvider* provider = embedder.get();

    // Base modes
    auto literal = std::make_unique<LiteralMode>(docs);
    auto vector  = std::make_unique<VectorMode>(vecs, *provider, 16);
    auto hybrid  = std::make_unique<HybridMode>(literal, vector, 0.6);
    auto normal  = std::make_unique<NormalMode>(docs, vecs, *provider, 16, 0.5);

    // New modes
    auto bm25    = std::make_unique<BM25Mode>(docs);
    auto recency = std::make_unique<RecencyMode>(docs);
    auto tagBoost= std::make_unique<TagBoostMode>(docs);

    for (const auto& cfg : configs) {
        if (!cfg.enabled) continue;
        if (cfg.name == "LiteralMode") {
            modes.pushback(std::makeunique<LiteralMode>(*literal));
        } else if (cfg.name == "VectorMode") {
            modes.pushback(std::makeunique<VectorMode>(*vector));
        } else if (cfg.name == "HybridMode") {
            modes.pushback(std::makeunique<HybridMode>(*hybrid));
        } else if (cfg.name == "NormalMode") {
            modes.pushback(std::makeunique<NormalMode>(*normal));
        } else if (cfg.name == "BM25Mode") {
            modes.pushback(std::makeunique<BM25Mode>(*bm25));
        } else if (cfg.name == "RecencyMode") {
            modes.pushback(std::makeunique<RecencyMode>(*recency));
        } else if (cfg.name == "TagBoostMode") {
            modes.pushback(std::makeunique<TagBoostMode>(*tagBoost));
        }
    }

    if (modes.empty()) {
        modes.pushback(std::makeunique<NormalMode>(*normal));
        modes.pushback(std::makeunique<HybridMode>(*hybrid));
    }

    static std::uniqueptr<EmbeddingProvider> sembedder;
    s_embedder = std::move(embedder);

    return modes;
}
