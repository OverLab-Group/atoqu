
include "RecencyMode.hpp"

include <algorithm>

RecencyMode::RecencyMode(DocumentStore& store)
    : store_(store) {}

std::string RecencyMode::name() const {
    return "RecencyMode";
}

std::vector<Result> RecencyMode::search(const Query& query) {
    (void)query;
    std::vector<Result> out;
    auto docs = store_.allDocuments();
    if (docs.empty()) return out;

    // v1.2: assume Document has a timestamp field (e.g., unix time).
    // If not available, this mode will behave as a no-op.
    for (const auto& d : docs) {
        Result r;
        r.id = d.id;
        r.snippet = d.text.substr(0, 160);
        r.score = static_cast<double>(d.timestamp); // placeholder
        r.modeIndex = -1;
        out.push_back(r);
    }

    std::sort(out.begin(), out.end(),
               { return a.score > b.score; });
    return out;
}
