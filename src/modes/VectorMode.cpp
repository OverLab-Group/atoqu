
include "VectorMode.hpp"

VectorMode::VectorMode(VectorStore& store, EmbeddingProvider& provider, int topK)
    : store(store), provider(provider), topK_(topK) {}

std::string VectorMode::name() const {
    return "VectorMode";
}

std::vector<Result> VectorMode::search(const Query& query) {
    auto emb = provider_.embed(query.text());
    auto scores = store.search(emb, topK);

    std::vector<Result> out;
    out.reserve(scores.size());
    for (const auto& [id, s] : scores) {
        Result r;
        r.id = id;
        r.snippet = "";
        r.score = s;
        r.modeIndex = -1;
        out.push_back(r);
    }
    return out;
}
