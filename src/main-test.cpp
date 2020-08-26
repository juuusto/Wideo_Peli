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

TEST_CASE("Test that get tile works. Test that get map,tile and grid works normally.","[map]") {
    Map map("map3.map");
    REQUIRE(map.getMapHeight() != 0);
    CHECK(map.getMapMusicFile() != "");
    CHECK(map.getTileId(0,0) == 1);
    map.setTile(0,0,20);
    CHECK(map.getTileId(0,0) == 20);
    CHECK(map.getMapWidth() != 0);
    CHECK(map.getTiles().size() > 0);
    CHECK(map.getBlockSize() != 0);
    CHECK(map.getTile(1).getTileName() != "");
    CHECK(map.getTile(1).getCollArr().size() > 0);
    CHECK(map.getTile(1).isCollision(0,0) == true);
    CHECK(map.getGrid().getSize().first >=0);
    CHECK(map.getGrid().getSize().second >=0);
    CHECK(map.getGrid().getGrid().size() > 0);
    CHECK(map.getGrid().getTileAtCoordinate(0,0) != -1);
    CHECK_NOTHROW(map.getGrid().setTileAtCoordinate(0,0,0));
    CHECK(map.getGrid().getTileAtCoordinate(0,0) >=0);


}

TEST_CASE("Test vehicles and player", "[vehicle]") {
    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);
    Player player("test player", "",ajoneuvo);

    CHECK(ajoneuvo.getMaxSpeed() != 0);
    CHECK(ajoneuvo.getSprite() != "");
    CHECK(ajoneuvo.getTurnSpeed() != 0);
    CHECK(ajoneuvo.getDrag() != 0);
    CHECK(ajoneuvo.getAcceleration() > 0);    

    CHECK(player.getLocation().first >= 0);
    CHECK(player.getLocation().second >= 0);
    CHECK(player.getName() == "test player");
    CHECK_NOTHROW(player.updateLocation());
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

    CHECK(aa.connect(pelaaja) == true);

    CHECK(aa.getServerMap() != "");
    CHECK(aa.getPlayerCount() == 0);
    CHECK(aa.disconnect() == false);
    CHECK(aa.isConnected() == true);
    //not connected, so should be 0
    CHECK(aa.getConId() == 0);
}

