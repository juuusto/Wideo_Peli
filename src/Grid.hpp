#include <algorithm>
#include <vector>

class Grid {
public:
    Grid();
    Grid(int width, int height) : width_(width), height_(height) {}
    ~Grid(){
        map.clear();
    };


    std::pair<int,int> getSize();

    int getTileAtCoordinate(int x, int y);

    std::vector<int>& getGrid();

    void setGrid();

    void setTileAtCoordinate(int x, int y,int value); 

private:
    int width_;
    int height_;

    std::vector<int> map;
};