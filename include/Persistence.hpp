
pragma once

include <string>

include <vector>

class DocumentStore;
class VectorStore;

namespace Persistence {
    bool saveDocuments(const std::string& path, const DocumentStore& store);
    bool loadDocuments(const std::string& path, DocumentStore& store);

    bool saveVectors(const std::string& path, const VectorStore& store);
    bool loadVectors(const std::string& path, VectorStore& store);

    bool saveModeWeights(const std::string& path, const std::vector<double>& weights);
    bool loadModeWeights(const std::string& path, std::vector<double>& weights);
}
