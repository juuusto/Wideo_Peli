#include "Tile.hpp"
#include "Grid.hpp"
#include <vector>

class Map {
public:
    Map(std::vector<Tile> tileArr, int rectSize, int x, int y, std::vector<int> idArray) :
    tileArr_(tileArr), rectSize_(rectSize), width(x), height(y), idArray_(idArray) {

        grid = Grid(x, y);
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
    std::vector<int> idArray_;
    Grid grid;
};