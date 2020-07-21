#include "Map.hpp"

int Map::getMapHeight() {return height;}

int Map::getMapWidth() {return width;}

void Map::constructMap() {
    std::pair<int,int> size = grid.getSize();
    for(int x = 0; x < size.first; x++) {
        for (int y = 0; y < size.second; y++) {
            grid.setTileAtCoordinate(x,y, idArray_[y*size.first + x]);
        }
    }
}

Tile Map::getTile(int id) { return tileArr_[id];}

Tile Map::getTile(int x, int y) { return tileArr_[grid.getTileAtCoordinate(x,y)];}