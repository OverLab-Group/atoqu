
pragma once

include <memory>

include <vector>

include <string>

include "SearchMode.hpp"

include "Config.hpp"

class DocumentStore;
class VectorStore;

namespace ModeFactory {
    std::vector<std::unique_ptr<SearchMode>> createModes(
        const std::vector<ModeConfig>& configs,
        const DocumentStore& docs,
        VectorStore& vec
    );
}
