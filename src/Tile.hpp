#ifndef __GRID_H__
#define __GRID_H__
#include<string>
class Tile {
public:
    Tile(std::string tilename): tilename_(tilename){};
    std::string getTileName(){
        return tilename_;
    }

    //TODO: collisions
    bool isCollision(int x, int y){
        return false;//x ==199;
    }
private:
std::string tilename_;

};
#endif