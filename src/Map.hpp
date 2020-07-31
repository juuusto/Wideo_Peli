#include "Tile.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "Grid.cpp"
#include <vector>

std::vector<char> loadFile(std::string fname ){
    FILE *f = fopen(fname.c_str(), "rb");
    std::vector<char> t1;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);

    if (fsize>5000) return t1;
    fseek(f, 0, SEEK_SET);
    char tmp = 0;
    for(int i = 0; i< fsize; i++){
        fread(&tmp, 1, 1, f);
        t1.push_back(tmp);
    }
    fclose(f);
    return t1;
}

class Map {
public:
    Map(std::vector<Tile> tileArr, int rectSize, int x, int y, std::vector<int> mapArray) :
    tileArr_(tileArr), rectSize_(rectSize), width(x), height(y), mapArray_(mapArray) {

        grid = Grid(x, y);
        grid.setGrid();
    }

    Map(std::string fname) {
        char byte;
        int val;
        int idArraySize;
        rectSize_=200;
        std::ifstream mapf ("./assets/"+fname);
        
        if(mapf.is_open()) {
            
            //Read to width(x). Size is 2 bytes.
            mapf.read(&byte, 1);
            val = byte << 8;
            mapf.read(&byte, 1);
            width = val | byte;
            
            //Read to height(y). Size is 2 bytes.
            mapf.read(&byte, 1);
            val = byte << 8;
            mapf.read(&byte, 1);
            height = val | byte;
            
            grid = Grid(width, height);
            grid.setGrid();
            mapf.read(&byte, 1);
            idArraySize = byte;

            while(idArraySize > 0) {
                int id;
                mapf.read(&byte, 1);
                id = byte;

                tileArr_.push_back(Tile("assets/tile"+std::to_string(id)+".png",loadFile("assets/tile"+std::to_string(id)+".dat")));

                id = 0;
                idArraySize--;
            }

            int mapDataSize = width*height;
            while(mapDataSize > 0) {
                int id;
                mapf.read(&byte, 1);
                id = byte;
                mapArray_.push_back(id);
                id = 0;
                mapDataSize--;
            }

            this->constructMap();
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