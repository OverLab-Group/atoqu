
pragma once

include <string>

include <vector>

include "Query.hpp"

include "Result.hpp"

/
 * @brief Abstract base class for search modes.
 */
class SearchMode {
public:
    virtual ~SearchMode() = default;
    virtual std::string name() const = 0;
    virtual std::vector<Result> search(const Query& query) = 0;
};
