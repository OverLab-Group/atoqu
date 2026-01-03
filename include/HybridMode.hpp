
pragma once

include "SearchMode.hpp"

include "LiteralMode.hpp"

include "VectorMode.hpp"

/
 * @brief Hybrid mode combining literal and vector scores.
 */
class HybridMode : public SearchMode {
public:
    HybridMode(LiteralMode& literal, VectorMode& vector, double alpha);

    std::string name() const override;
    std::vector<Result> search(const Query& query) override;

private:
    LiteralMode& literal_;
    VectorMode& vector_;
    double alpha_;
};
