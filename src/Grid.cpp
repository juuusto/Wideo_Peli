#include "Grid.hpp"

std::pair<int,int> Grid::getSize() {return std::make_pair (width_, height_);}

int Grid::getTile( int x, int y) {
    return map[x][y];
}


std::vector<std::vector<int>>& Grid::getGrid() { return map;}


void Grid::setGrid() {
    for( int x = 0; x < width_; x++)
    {
        for( int y = 0; y < height_; y++)
        {
            map[x][y] = 0;
        }
    }
}

void Grid::setTile(int x, int y, int value) { map[x][y] = value;}