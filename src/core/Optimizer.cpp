
include "Optimizer.hpp"

include "Json.hpp"

include "FileSystem.hpp"

include "StringUtil.hpp"

using atoqu::Json;
using atoqu::FileSystem;
using atoqu::StringUtil;

Optimizer::Optimizer(const std::string& profilePath)
    : profilePath_(profilePath) {}

std::string Optimizer::makeKey(const std::string& query) const {
    std::string key = StringUtil::toLower(query);
    if (key.size() > 32) key = key.substr(0, 32);
    return key;
}

void Optimizer::load() {
    profiles_.clear();
    std::string content;
    if (!FileSystem::readFile(profilePath_, content)) {
        return;
    }
    Json root = Json::parse(content);
    if (!root.isArray()) return;

    for (const auto& item : root.asArray()) {
        if (!item.isObject()) continue;
        const auto& obj = item.asObject();
        QueryProfile p;
        auto itPattern = obj.find("pattern");
        auto itWeights = obj.find("modeWeights");
        auto itSamples = obj.find("samples");
        if (itPattern == obj.end() || itWeights == obj.end() || itSamples == obj.end()) continue;

        p.pattern = itPattern->second.asString();
        p.samples = staticcast<std::sizet>(itSamples->second.asNumber());

        if (itWeights->second.isArray()) {
            for (const auto& w : itWeights->second.asArray()) {
                p.modeWeights.push_back(w.asNumber());
            }
        }

        if (!p.pattern.empty()) {
            profiles_[p.pattern] = p;
        }
    }
}

void Optimizer::save() const {
    Json root = Json::array_t{};
    for (const auto& [k, p] : profiles_) {
        Json obj = Json::object_t{};
        obj["pattern"] = p.pattern;
        Json weights = Json::array_t{};
        for (double w : p.modeWeights) {
            weights.asArray().push_back(Json(w));
        }
        obj["modeWeights"] = weights;
        obj["samples"] = static_cast<double>(p.samples);
        root.asArray().push_back(obj);
    }
    std::string out = root.dump(2);
    FileSystem::writeFile(profilePath_, out);
}

std::vector<double> Optimizer::adjustWeights(const std::string& query,
                                             const std::vector<double>& currentWeights) const {
    auto key = makeKey(query);
    auto it = profiles_.find(key);
    if (it == profiles_.end()) return currentWeights;
    const auto& p = it->second;
    if (p.modeWeights.size() != currentWeights.size()) return currentWeights;
    return p.modeWeights;
}

void Optimizer::recordFeedback(const std::string& query, int modeIndex,
                               std::vector<double>& currentWeights) {
    if (modeIndex < 0 || modeIndex >= static_cast<int>(currentWeights.size())) return;

    auto key = makeKey(query);
    auto it = profiles_.find(key);
    if (it == profiles_.end()) {
        QueryProfile p;
        p.pattern = key;
        p.modeWeights = currentWeights;
        p.samples = 0;
        profiles_[key] = p;
        it = profiles_.find(key);
    }

    auto& p = it->second;
    p.samples += 1;

    for (std::size_t i = 0; i < p.modeWeights.size(); ++i) {
        if (static_cast<int>(i) == modeIndex) {
            p.modeWeights[i] = (p.modeWeights[i] * (p.samples - 1) + 1.0) / p.samples;
        } else {
            p.modeWeights[i] = (p.modeWeights[i] * (p.samples - 1) + 0.0) / p.samples;
        }
    }
}
