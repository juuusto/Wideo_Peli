#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include "Map.cpp"
#include "vehicle.hpp"
#include "player.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <SFML/Network.hpp>
#include "projectile.hpp"
#include "network.cpp"

TEST_CASE("Test that get tile works","[map]") {
    Map map("map3.map");
    REQUIRE(map.getMapHeight() != 0);
    CHECK(map.getMapMusicFile() != "");
    CHECK(map.getTileId(0,0) == 1);
    map.setTile(0,0,20);
    CHECK(map.getTileId(0,0) == 20);
    CHECK(map.getMapWidth() != 0);
    CHECK(map.getTiles().size() > 0);
}

TEST_CASE("Test vehicles and player", "[vehicle]") {
    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);
    Player player("test player", "",ajoneuvo);

    CHECK(ajoneuvo.getMaxSpeed() != 0);
    CHECK(ajoneuvo.getSprite() != "");
    CHECK(ajoneuvo.getTurnSpeed() != 0);

    CHECK(player.getLocation().first != -1);
    CHECK(player.getLocation().second != -1);
    CHECK(player.getAmmo() == 20);
    player.shoot();
    CHECK(player.getAmmo() == 19);
    CHECK(player.getHp() != 0);
    int hp = player.getHp();
    for(int i = 0; i < hp; i++)
    {
        player.hit();
    }
    CHECK(player.getHp() == 0);
}

TEST_CASE("Test if network works properly.","[network]" ) {

    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);

    Player pelaaja("test_player", "", ajoneuvo);

    Network aa("127.0.0.1");

    aa.connect(pelaaja);

    CHECK(aa.getServerMap() != "");
    CHECK(aa.getPlayerCount() == 1);
    CHECK(aa.disconnect() == false);

}