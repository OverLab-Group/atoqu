
pragma once

include "SearchMode.hpp"

include "DocumentStore.hpp"

/
 * @brief Tag-based boosting mode.
 *
 * v1.2: boosts documents whose tags match query tokens.
 */
class TagBoostMode : public SearchMode {
public:
    explicit TagBoostMode(DocumentStore& store);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    DocumentStore& store_;
};
