
define CATCHCONFIGMAIN

include <catch2/catch.hpp>

include "DocumentStore.hpp"

TEST_CASE("DocumentStore basic add/get", "[docstore]") {
    DocumentStore store;
    store.addDocument("id1", "hello world");
    auto* d = store.get("id1");
    REQUIRE(d != nullptr);
    REQUIRE(d->text == "hello world");
}
