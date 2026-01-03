
include "AtoquEngine.hpp"

include "Query.hpp"

int main() {
    AtoquEngine engine;
    Query q("security smoke test");
    auto results = engine.search(q);
    (void)results;
    return 0;
}
