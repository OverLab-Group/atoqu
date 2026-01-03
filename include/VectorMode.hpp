
pragma once

include "SearchMode.hpp"

include "VectorStore.hpp"

include "EmbeddingProvider.hpp"

/
 * @brief Vector-based semantic-ish search mode.
 */
class VectorMode : public SearchMode {
public:
    VectorMode(VectorStore& store, EmbeddingProvider& provider, int topK);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    VectorStore& store_;
    EmbeddingProvider& provider_;
    int topK_;
};
