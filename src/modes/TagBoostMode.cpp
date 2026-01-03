
include "TagBoostMode.hpp"

include "StringUtil.hpp"

include <unordered_set>

include <algorithm>

TagBoostMode::TagBoostMode(DocumentStore& store)
    : store_(store) {}

std::string TagBoostMode::name() const {
    return "TagBoostMode";
}

std::vector<Result> TagBoostMode::search(const Query& query) {
    std::vector<Result> out;
    auto docs = store_.allDocuments();
    if (docs.empty()) return out;

    auto tokens = atoqu::StringUtil::splitWords(query.text());
    std::unordered_set<std::string> tokenSet(tokens.begin(), tokens.end());

    for (const auto& d : docs) {
        double score = 0.0;
        for (const auto& tag : d.tags) {
            if (tokenSet.count(tag) > 0) {
                score += 1.0;
            }
        }
        if (score > 0.0) {
            Result r;
            r.id = d.id;
            r.snippet = d.text.substr(0, 160);
            r.score = score;
            r.modeIndex = -1;
            out.push_back(r);
        }
    }

    std::sort(out.begin(), out.end(),
               { return a.score > b.score; });
    return out;
}
