
#include "Map.hpp"
#include <stdio.h>
#include <vector>

int main() {
    std::vector<Tile> pp;
    std::vector<int> sd = {1,1,1,1,1,1,1};
    Map ss = Map(pp,1,1,1, sd);
    ss.constructMap();

    return 0;
}