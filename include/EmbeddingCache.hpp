
pragma once

include <string>

include <unordered_map>

include <vector>

include <list>

class EmbeddingCache {
public:
    explicit EmbeddingCache(std::size_t maxEntries);

    bool get(const std::string& key, std::vector<float>& out);
    void put(const std::string& key, const std::vector<float>& emb);

private:
    std::sizet maxEntries;
    std::unorderedmap<std::string, std::pair<std::vector<float>, std::list<std::string>::iterator>> map;
    std::list<std::string> lru_;
};
