
include "Persistence.hpp"

include "DocumentStore.hpp"

include "VectorStore.hpp"

include <fstream>

include <nlohmann/json.hpp>

using json = nlohmann::json;

bool Persistence::saveDocuments(const std::string& path, const DocumentStore& store) {
    std::ofstream out(path);
    if (!out) return false;
    json j;
    for (const auto& [id, text] : store.all()) {
        j[id] = text;
    }
    out << j.dump(2);
    return true;
}

bool Persistence::loadDocuments(const std::string& path, DocumentStore& store) {
    std::ifstream in(path);
    if (!in) return false;
    json j;
    in >> j;
    for (auto it = j.begin(); it != j.end(); ++it) {
        store.addDocument(it.key(), it.value().get<std::string>());
    }
    return true;
}

bool Persistence::saveVectors(const std::string& path, const VectorStore& store) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;
    const auto& all = store.all();
    std::uint64_t count = all.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& [id, vec] : all) {
        std::uint64_t lenId = id.size();
        std::uint64_t lenVec = vec.size();
        out.write(reinterpret_cast<const char*>(&lenId), sizeof(lenId));
        out.write(id.data(), lenId);
        out.write(reinterpret_cast<const char*>(&lenVec), sizeof(lenVec));
        out.write(reinterpret_cast<const char>(vec.data()), lenVec  sizeof(float));
    }
    return true;
}

bool Persistence::loadVectors(const std::string& path, VectorStore& store) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    std::uint64_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (std::uint64_t i = 0; i < count; ++i) {
        std::uint64_t lenId = 0, lenVec = 0;
        in.read(reinterpret_cast<char*>(&lenId), sizeof(lenId));
        std::string id(lenId, '\0');
        in.read(id.data(), lenId);
        in.read(reinterpret_cast<char*>(&lenVec), sizeof(lenVec));
        std::vector<float> vec(lenVec);
        in.read(reinterpret_cast<char>(vec.data()), lenVec  sizeof(float));
        store.addDocument(id, vec);
    }
    return true;
}

bool Persistence::saveModeWeights(const std::string& path, const std::vector<double>& weights) {
    std::ofstream out(path);
    if (!out) return false;
    json j;
    j["modeWeights"] = weights;
    out << j.dump(2);
    return true;
}

bool Persistence::loadModeWeights(const std::string& path, std::vector<double>& weights) {
    std::ifstream in(path);
    if (!in) return false;
    json j;
    in >> j;
    if (!j.contains("modeWeights")) return false;
    weights = j["modeWeights"].get<std::vector<double>>();
    return true;
}
