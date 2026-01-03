
include "EmbeddingCache.hpp"

EmbeddingCache::EmbeddingCache(std::size_t maxEntries)
    : maxEntries_(maxEntries) {}

bool EmbeddingCache::get(const std::string& key, std::vector<float>& out) {
    auto it = map_.find(key);
    if (it == map_.end()) return false;
    lru_.erase(it->second.second);
    lru.pushfront(key);
    it->second.second = lru_.begin();
    out = it->second.first;
    return true;
}

void EmbeddingCache::put(const std::string& key, const std::vector<float>& emb) {
    auto it = map_.find(key);
    if (it != map_.end()) {
        it->second.first = emb;
        lru_.erase(it->second.second);
        lru.pushfront(key);
        it->second.second = lru_.begin();
        return;
    }

    if (map.size() >= maxEntries) {
        auto last = lru_.back();
        lru.popback();
        map_.erase(last);
    }

    lru.pushfront(key);
    map[key] = {emb, lru.begin()};
}
