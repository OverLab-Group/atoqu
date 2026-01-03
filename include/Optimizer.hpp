
pragma once

include <string>

include <unordered_map>

include <vector>

struct QueryProfile {
    std::string pattern;
    std::vector<double> modeWeights;
    std::size_t samples = 0;
};

class Optimizer {
public:
    Optimizer(const std::string& profilePath);

    void load();
    void save() const;

    std::vector<double> adjustWeights(const std::string& query,
                                      const std::vector<double>& currentWeights) const;

    void recordFeedback(const std::string& query, int modeIndex,
                        std::vector<double>& currentWeights);

private:
    std::string profilePath_;
    mutable std::unorderedmap<std::string, QueryProfile> profiles;

    std::string makeKey(const std::string& query) const;
};
