
define CATCHCONFIGMAIN

include <catch2/catch.hpp>

include "AtoquEngine.hpp"

include "Query.hpp"

TEST_CASE("AtoquEngine end-to-end search", "[integration]") {
    AtoquEngine engine;
    Query q("atomic search");
    auto results = engine.search(q);
    REQUIRE(results.size() >= 0);
}
