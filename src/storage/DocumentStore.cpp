
include "DocumentStore.hpp"

bool DocumentStore::addDocument(const std::string& id,
                                const std::string& text,
                                const std::vector<std::string>& tags,
                                const std::string& source) {
    DocumentMeta meta;
    meta.id = id;
    meta.text = text;
    meta.tags = tags;
    meta.source = source;
    docs_[id] = meta;
    return true;
}

bool DocumentStore::removeDocument(const std::string& id) {
    return docs_.erase(id) > 0;
}

const DocumentMeta* DocumentStore::get(const std::string& id) const {
    auto it = docs_.find(id);
    if (it == docs_.end()) return nullptr;
    return &it->second;
}

std::vector<DocumentMeta> DocumentStore::allDocuments() const {
    std::vector<DocumentMeta> out;
    out.reserve(docs_.size());
    for (const auto& [id, meta] : docs_) {
        out.push_back(meta);
    }
    return out;
}
