
pragma once

include "SearchMode.hpp"

include "DocumentStore.hpp"

/
 * @brief Literal keyword-based search mode.
 */
class LiteralMode : public SearchMode {
public:
    explicit LiteralMode(DocumentStore& store);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    DocumentStore& store_;
};
