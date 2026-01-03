
include "LiteralMode.hpp"

include "StringUtil.hpp"

LiteralMode::LiteralMode(DocumentStore& store)
    : store_(store) {}

std::string LiteralMode::name() const {
    return "LiteralMode";
}

std::vector<Result> LiteralMode::search(const Query& query) {
    std::vector<Result> out;
    auto docs = store_.allDocuments();
    std::string q = atoqu::StringUtil::toLower(query.text());

    for (const auto& d : docs) {
        std::string text = atoqu::StringUtil::toLower(d.text);
        if (text.find(q) != std::string::npos) {
            Result r;
            r.id = d.id;
            r.snippet = d.text.substr(0, 160);
            r.score = 1.0;
            r.modeIndex = -1;
            out.push_back(r);
        }
    }
    return out;
}
