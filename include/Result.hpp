
pragma once

include <string>

struct Result {
    std::string id;
    std::string snippet;
    double score;
    int modeIndex = -1; // which mode produced this result
};
