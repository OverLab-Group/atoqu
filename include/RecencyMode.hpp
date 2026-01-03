
pragma once

include "SearchMode.hpp"

include "DocumentStore.hpp"

/
 * @brief Recency-based ranking mode.
 *
 * v1.2: boosts documents with newer timestamps (if available).
 */
class RecencyMode : public SearchMode {
public:
    explicit RecencyMode(DocumentStore& store);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    DocumentStore& store_;
};
