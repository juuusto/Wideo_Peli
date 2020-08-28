#include "Map.hpp"

int Map::getMapHeight() { return height; }

int Map::getMapWidth() { return width; }

void Map::constructMap()
{
    std::pair<int, int> size = grid.getSize();
    for (int x = 0; x < size.first; x++)
    {
        for (int y = 0; y < size.second; y++)
        {
            grid.setTileAtCoordinate(x, y, mapArray_[y * size.first + x]);
        }
    }
}

int Map::getBlockSize()
{
    return rectSize_;
}

Tile Map::getTile(int id) { return tileArr_[id]; }

Tile Map::getTile(int x, int y) { return tileArr_[grid.getTileAtCoordinate(x, y)]; }

std::string Map::getMapMusicFile() { return musicFname; }

Grid Map::getGrid() {return grid;}

int Map::getTileId(int x, int y)
{
    return grid.getTileAtCoordinate(x, y);
}
int Map::getTileCheckpoint(int x, int y)
{
    return grid.getTileCheckpointAt(x, y);
}

void Map::setTile(int x, int y, int value)
{
    grid.setTileAtCoordinate(x, y, value);
}

std::vector<Tile> Map::getTiles()
{
    return tileArr_;
}
std::pair<int,int> Map::getStartLoc(){
    return std::pair<int,int>(startx,starty);
}