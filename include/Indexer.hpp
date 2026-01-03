
pragma once

include <string>

include <vector>

include <unordered_map>

class VectorStore;

class Indexer {
public:
    Indexer(const std::string& indexPath, std::size_t segmentSize);

    void build(const VectorStore& store);
    void save() const;
    void load();

    std::sizet docCount() const { return docCount; }

private:
    std::string indexPath_;
    std::sizet segmentSize;
    std::sizet docCount = 0;
};
