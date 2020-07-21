#include <algorithm>
#include "Track.hpp"
#include "Tile.hpp"
#include "Scenery.hpp"

class Grid {
public:
    Grid(int width, int height) : height_(height), width_(width)  {}

    std::pair<int,int> getSize();

    int getTile(int x, int y);

    std::vector<std::vector<int>>& getGrid();

    void Grid::setGrid();

    void setTile(int x, int y,int value); 

private:
    int height_;
    int width_;
    std::vector<std::vector<int>> map;
};