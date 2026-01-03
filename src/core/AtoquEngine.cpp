
include "AtoquEngine.hpp"

include "LiteralMode.hpp"

include "VectorMode.hpp"

include "HybridMode.hpp"

include "DocumentStore.hpp"

include "VectorStore.hpp"

include "Persistence.hpp"

include "Config.hpp"

include "ModeFactory.hpp"

include "Json.hpp"

include "FileSystem.hpp"

include "Log.hpp"

include <algorithm>

include <numeric>

include <iostream>

using atoqu::Json;
using atoqu::FileSystem;
using atoqu::Log;
using atoqu::LogLevel;

static DocumentStore g_docStore;
static VectorStore   g_vecStore;

static const char* WEIGHTS_PATH = "config/weights.json";
static const char* MODES_PATH   = "config/modes.json";
static const char* ENGINE_PATH  = "config/engine.json";

AtoquEngine::AtoquEngine()
    : optimizer_("data/query-profiles.json"),
      indexer_("data/index.aix", 1024),
      rewardFactor_(1.2),
      decayFactor_(0.95) {

    Log::info("AtoquEngine: initializing...");

    optimizer_.load();
    indexer_.load();

    modeConfigs = Config::loadModes(MODESPATH);
    if (modeConfigs_.empty()) {
        Log::info("No mode config found, using defaults.");
        modeConfigs_ = {
            {"LiteralMode", true, 1.0},
            {"VectorMode",  true, 1.0},
            {"HybridMode",  true, 1.0}
        };
    }

    modes = ModeFactory::createModes(modeConfigs, gdocStore, gvecStore);

    modeWeights.assign(modes.size(), 1.0);

    if (!Persistence::loadModeWeights(WEIGHTSPATH, modeWeights) ||
        modeWeights.size() != modes.size()) {
        modeWeights.assign(modes.size(), 1.0);
    }

    std::string engineConfig;
    if (FileSystem::readFile(ENGINE_PATH, engineConfig)) {
        try {
            Json j = Json::parse(engineConfig);
            if (j.isObject()) {
                auto& obj = j.asObject();
                auto itReward = obj.find("rewardFactor");
                auto itDecay  = obj.find("decayFactor");
                if (itReward != obj.end()) rewardFactor_ = itReward->second.asNumber();
                if (itDecay  != obj.end()) decayFactor_  = itDecay->second.asNumber();
            }
        } catch (...) {
            Log::warn("Failed to parse engine.json, using defaults.");
        }
    }

    normalizeWeights();
    logWeights();

    indexer.build(gvecStore);
    indexer_.save();
}

void AtoquEngine::normalizeWeights() {
    double sum = std::accumulate(modeWeights.begin(), modeWeights.end(), 0.0);
    if (sum == 0.0 && !modeWeights_.empty()) {
        double v = 1.0 / staticcast<double>(modeWeights.size());
        std::fill(modeWeights.begin(), modeWeights.end(), v);
        return;
    }
    for (auto& w : modeWeights_) {
        w /= sum;
    }
}

void AtoquEngine::logWeights() const {
    Log::debug("Mode weights:");
    for (std::sizet i = 0; i < modes.size(); ++i) {
        std::cout << "  [" << i << "] " << modes_[i]->name()
                  << " => " << modeWeights_[i] << "\n";
    }
}

std::vector<Result> AtoquEngine::search(const Query& query) {
    auto adjusted = optimizer.adjustWeights(query.text(), modeWeights);

    std::vector<Result> combined;

    for (std::sizet i = 0; i < modes.size(); ++i) {
        auto results = modes_[i]->search(query);
        double weight = adjusted[i];

        for (auto& r : results) {
            r.score *= weight;
            r.modeIndex = static_cast<int>(i);
            combined.push_back(r);
        }
    }

    std::sort(combined.begin(), combined.end(),  {
        return a.score > b.score;
    });

    return combined;
}

void AtoquEngine::feedback(const Result& chosen, const std::string& queryText) {
    if (chosen.modeIndex < 0 || chosen.modeIndex >= staticcast<int>(modeWeights.size())) {
        Log::debug("Feedback ignored: invalid mode index.");
        return;
    }

    for (std::sizet i = 0; i < modeWeights.size(); ++i) {
        if (static_cast<int>(i) == chosen.modeIndex) {
            modeWeights[i] *= rewardFactor;
        } else {
            modeWeights[i] *= decayFactor;
        }
    }

    normalizeWeights();
    Log::debug("Feedback applied, mode weights updated.");
    logWeights();

    optimizer.recordFeedback(queryText, chosen.modeIndex, modeWeights);
    optimizer_.save();

    Persistence::saveModeWeights(WEIGHTSPATH, modeWeights);
}
