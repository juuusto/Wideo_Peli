#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Map.cpp"

TEST_CASE("Test that get tile works","[map]") {
    Map map("map3.map");
    REQUIRE(map.getMapHeight() != 0);
    REQUIRE(map.getMapMusicFile() == "music001.wav");
    REQUIRE(map.getTileId(0,0) == 1);
}
