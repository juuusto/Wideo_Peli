#include <algorithm>
#include <vector>
#ifndef __GRID_H__
#define __GRID_H__

class Grid {
public:
    Grid();
    Grid(int width, int height) : height_(height), width_(width) {}

    std::pair<int,int> getSize();

    int getTileAtCoordinate(int x, int y);

    std::vector<int>& getGrid();

    void Grid::setGrid();

    void setTileAtCoordinate(int x, int y,int value); 

private:
    int height_;
    int width_;
    std::vector<int> map;
};
#endif