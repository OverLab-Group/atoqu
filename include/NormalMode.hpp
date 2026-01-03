
pragma once

include "SearchMode.hpp"

include "LiteralMode.hpp"

include "VectorMode.hpp"

include "HybridMode.hpp"

include "DocumentStore.hpp"

include "VectorStore.hpp"

/
 * @brief NormalMode: browser-friendly, "classic" search mode.
 *
 * Combines:
 *  - Literal keyword matching
 *  - Vector similarity
 *  - Hybrid scoring
 *
 * With:
 *  - Atomic/quantum-inspired resource usage
 *  - Lightweight scoring pipeline
 */
class NormalMode : public SearchMode {
public:
    NormalMode(DocumentStore& docs,
               VectorStore& vecs,
               EmbeddingProvider& provider,
               int topK,
               double alpha);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    DocumentStore& docs_;
    VectorStore& vecs_;
    EmbeddingProvider& provider_;
    int topK_;
    double alpha_;
};
