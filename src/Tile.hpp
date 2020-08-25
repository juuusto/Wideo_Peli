#ifndef __GRID_H__
#define __GRID_H__
#include<string>
#include <vector>
class Tile {
public:
    Tile(std::string tilename,std::vector<char> collisions ): tilename_(tilename),collisions_(collisions){
        
    };
    std::string getTileName(){
        return tilename_;
    }

    //TODO: collisions
    bool isCollision(int x, int y){
        int xC = x/8;
        int xB = x-xC*8;
        return collisions_[xC +25*y]&(1<<xB) != 0;
    }

    std::vector<char>  getCollArr (){
        return collisions_;
    }
private:
std::string tilename_;
std::vector<char>  collisions_;
};
#endif