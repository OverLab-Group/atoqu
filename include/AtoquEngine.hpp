
pragma once

include <memory>

include <vector>

include <string>

include "Query.hpp"

include "Result.hpp"

include "SearchMode.hpp"

include "Config.hpp"

include "Optimizer.hpp"

include "Indexer.hpp"

class AtoquEngine {
public:
    AtoquEngine();

    std::vector<Result> search(const Query& query);
    void feedback(const Result& chosen, const std::string& queryText);

    const std::vector<double>& modeWeights() const { return modeWeights_; }

private:
    std::vector<std::uniqueptr<SearchMode>> modes;
    std::vector<double> modeWeights_;
    std::vector<ModeConfig> modeConfigs_;

    Optimizer optimizer_;
    Indexer indexer_;

    double rewardFactor_;
    double decayFactor_;

    void normalizeWeights();
    void logWeights() const;
};
