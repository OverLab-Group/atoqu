
include "NormalMode.hpp"

include "StringUtil.hpp"

include <unordered_map>

include <algorithm>

NormalMode::NormalMode(DocumentStore& docs,
                       VectorStore& vecs,
                       EmbeddingProvider& provider,
                       int topK,
                       double alpha)
    : docs_(docs),
      vecs_(vecs),
      provider_(provider),
      topK_(topK),
      alpha_(alpha) {}

std::string NormalMode::name() const {
    return "NormalMode";
}

std::vector<Result> NormalMode::search(const Query& query) {
    // Atomic step 1: literal match
    std::vector<Result> literalResults;
    {
        std::string q = atoqu::StringUtil::toLower(query.text());
        auto docs = docs_.allDocuments();
        for (const auto& d : docs) {
            std::string text = atoqu::StringUtil::toLower(d.text);
            if (text.find(q) != std::string::npos) {
                Result r;
                r.id = d.id;
                r.snippet = d.text.substr(0, 160);
                r.score = 1.0;
                r.modeIndex = -1;
                literalResults.push_back(r);
            }
        }
    }

    // Atomic step 2: vector match
    std::vector<Result> vectorResults;
    {
        auto emb = provider_.embed(query.text());
        auto scores = vecs.search(emb, topK);
        vectorResults.reserve(scores.size());
        for (const auto& [id, s] : scores) {
            Result r;
            r.id = id;
            r.snippet = "";
            r.score = s;
            r.modeIndex = -1;
            vectorResults.push_back(r);
        }
    }

    // Quantum-like combination: merge scores
    std::unordered_map<std::string, Result> merged;

    for (auto& r : literalResults) {
        merged[r.id] = r;
    }

    for (auto& r : vectorResults) {
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
