
include "HybridMode.hpp"

include <unordered_map>

include <algorithm>

HybridMode::HybridMode(LiteralMode& literal, VectorMode& vector, double alpha)
    : literal(literal), vector(vector), alpha_(alpha) {}

std::string HybridMode::name() const {
    return "HybridMode";
}

std::vector<Result> HybridMode::search(const Query& query) {
    auto lit = literal_.search(query);
    auto vec = vector_.search(query);

    std::unordered_map<std::string, Result> merged;

    for (auto& r : lit) {
        merged[r.id] = r;
    }

    for (auto& r : vec) {
        auto it = merged.find(r.id);
        if (it == merged.end()) {
            merged[r.id] = r;
        } else {
            it->second.score = alpha  it->second.score + (1.0 - alpha)  r.score;
        }
    }

    std::vector<Result> out;
    out.reserve(merged.size());
    for (auto& [id, r] : merged) {
        out.push_back(r);
    }

    std::sort(out.begin(), out.end(),  {
        return a.score > b.score;
    });

    return out;
}
