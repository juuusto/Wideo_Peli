#include "Tile.hpp"
#include "Grid.hpp"

class Map {
public:
    Map(std::vector<Tile> tileArr, int rectSize, int x, int y) :
    tileArr_(tileArr), rectSize_(rectSize), width(x), height(y) {

        grid = Grid(x*rectSize_, y*rectSize_);
        grid.setGrid();
    }

    
    int getMapHeight(); 

    int getMapWidth();

    void constructMap();

    Tile getTile(int id);

    Tile getTile(int x, int y);


private:
    std::vector<Tile> tileArr_;
    int rectSize_;
    int width;
    int height;
    Grid grid;
};