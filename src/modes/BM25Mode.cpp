
include "BM25Mode.hpp"

include "StringUtil.hpp"

include <unordered_map>

include <cmath>

BM25Mode::BM25Mode(DocumentStore& store)
    : store_(store) {}

std::string BM25Mode::name() const {
    return "BM25Mode";
}

std::vector<Result> BM25Mode::search(const Query& query) {
    std::vector<Result> out;
    auto docs = store_.allDocuments();
    if (docs.empty()) return out;

    std::vector<std::string> terms = atoqu::StringUtil::splitWords(query.text());
    if (terms.empty()) return out;

    std::unordered_map<std::string, int> df;
    for (const auto& d : docs) {
        std::unordered_map<std::string, bool> seen;
        auto words = atoqu::StringUtil::splitWords(d.text);
        for (const auto& w : words) {
            if (!seen[w]) {
                df[w]++;
                seen[w] = true;
            }
        }
    }

    const double k1 = 1.5;
    const double b  = 0.75;
    double avgLen = 0.0;
    for (const auto& d : docs) {
        avgLen += static_cast<double>(d.text.size());
    }
    avgLen /= static_cast<double>(docs.size());

    for (const auto& d : docs) {
        auto words = atoqu::StringUtil::splitWords(d.text);
        std::unordered_map<std::string, int> tf;
        for (const auto& w : words) {
            tf[w]++;
        }
        double score = 0.0;
        for (const auto& t : terms) {
            int f = tf[t];
            if (f == 0) continue;
            int docFreq = df[t];
            if (docFreq == 0) continue;
            double idf = std::log((docs.size() - docFreq + 0.5) / (docFreq + 0.5) + 1.0);
            double len = static_cast<double>(d.text.size());
            double denom = f + k1  (1.0 - b + b  (len / avgLen));
            score += idf  (f  (k1 + 1.0) / denom);
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
