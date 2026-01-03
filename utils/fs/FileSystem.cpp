
include "FileSystem.hpp"

include <fstream>

namespace atoqu {

bool FileSystem::readFile(const std::string& path, std::string& out) {
    std::ifstream in(path);
    if (!in) return false;
    out.assign((std::istreambuf_iterator<char>(in)),
               std::istreambuf_iterator<char>());
    return true;
}

bool FileSystem::writeFile(const std::string& path, const std::string& data) {
    std::ofstream out(path);
    if (!out) return false;
    out << data;
    return true;
}

bool FileSystem::exists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

} // namespace atoqu
