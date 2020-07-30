#include "Tile.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "Grid.cpp"
#include <vector>

class Map {
public:
    Map(std::vector<Tile> tileArr, int rectSize, int x, int y, std::vector<int> mapArray) :
    tileArr_(tileArr), rectSize_(rectSize), width(x), height(y), mapArray_(idArray) {

        grid = Grid(x, y);
        grid.setGrid();
    }

    Map(std::string fname) {
        char byte;
        int val;
        int idArraySize;
        std::ifstream mapf (fname);
        
        if(mapf.is_open()) {
            
            //Read to width(x). Size is 2 bytes.
            mapf.read(&byte, 1);
            val = byte >> 8;
            mapf.read(&byte, 1);
            width = val || byte;
            
            //Read to height(y). Size is 2 bytes.
            mapf.read(&byte, 1);
            val = byte >> 8;
            mapf.read(&byte, 1);
            height = val || byte;

            mapf.read(&byte, 1);
            idArraySize = byte >> 8;

            while(idArraySize > 0) {
                int id;
                mapf.read(&byte, 1);
                id = byte >> 8;
                //TODO fill array with Tiles
                id = 0;
                idArraySize--;
            }

            int mapDataSize = width*height;
            while(mapDataSize > 0) {
                int id;
                mapf.read(&byte, 1);
                id = byte >> 8;
                mapArray_.push_back(id);
                id = 0;
                idArraySize--;
            }

        }
    }

    ~Map(){
        tileArr_.clear();
        mapArray_.clear();
    }
    
    int getMapHeight(); 

    int getMapWidth();

    void constructMap();

    int getBlockSize();

    Tile getTile(int id);

    Tile getTile(int x, int y);
    int getTileId(int x, int y);
    std::vector<Tile> getTiles();
    void setTile(int x, int y,int value);
private:
    std::vector<Tile> tileArr_;
    int rectSize_;
    int width;
    int height;
    std::vector<int> mapArray_;
    Grid grid;
};