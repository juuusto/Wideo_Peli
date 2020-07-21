#include "Grid.hpp"

std::pair<int,int> Grid::getSize() {return std::make_pair (width_, height_);}

int Grid::getTileAtCoordinate( int x, int y) {
    return map[x][y];
}


std::vector<int>& Grid::getGrid() { return map;}


void Grid::setGrid() {
    for( int i = 0; width_ *height_; i++) { map.push_back(0);}
}

void Grid::setTileAtCoordinate(int x, int y, int value) { map[y*width_ +x] = value;}