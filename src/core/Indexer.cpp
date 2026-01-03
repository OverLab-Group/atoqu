
include "Indexer.hpp"

include "VectorStore.hpp"

include "FileSystem.hpp"

include <fstream>

using atoqu::FileSystem;

Indexer::Indexer(const std::string& indexPath, std::size_t segmentSize)
    : indexPath(indexPath), segmentSize(segmentSize) {}

void Indexer::build(const VectorStore& store) {
    docCount_ = store.all().size();
}

void Indexer::save() const {
    std::ofstream out(indexPath_, std::ios::binary);
    if (!out) return;
    out.write(reinterpretcast<const char*>(&docCount), sizeof(docCount_));
}

void Indexer::load() {
    if (!FileSystem::exists(indexPath_)) return;
    std::ifstream in(indexPath_, std::ios::binary);
    if (!in) return;
    in.read(reinterpretcast<char*>(&docCount), sizeof(docCount_));
}
