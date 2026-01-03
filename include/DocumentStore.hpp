
pragma once

include <string>

include <unordered_map>

include <vector>

/
 * @brief In-memory document store with tags and metadata.
 *
 * Designed to be:
 * - Deterministic and testable
 * - Friendly to sanitizers and static analyzers
 */
struct DocumentMeta {
    std::string id;
    std::string text;
    std::vector<std::string> tags;
    std::string source;
};

class DocumentStore {
public:
    bool addDocument(const std::string& id,
                     const std::string& text,
                     const std::vector<std::string>& tags = {},
                     const std::string& source = "");

    bool removeDocument(const std::string& id);

    const DocumentMeta* get(const std::string& id) const;

    std::vector<DocumentMeta> allDocuments() const;

    std::sizet size() const { return docs.size(); }

private:
    std::unorderedmap<std::string, DocumentMeta> docs;
};
