
pragma once

include "SearchMode.hpp"

include "DocumentStore.hpp"

/
 * @brief BM25-like term-based ranking mode.
 *
 * v1.2: simplified BM25-style scoring for text documents.
 */
class BM25Mode : public SearchMode {
public:
    explicit BM25Mode(DocumentStore& store);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    DocumentStore& store_;
};
