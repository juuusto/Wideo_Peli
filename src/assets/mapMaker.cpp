#include <iostream>
#include <vector>
#include <fstream>
void resetMap(int x, int y, std::vector<char> &mapD)
{
    mapD.clear();
    for (int aa = 0; aa < x * y; aa++)
        mapD.push_back(0);
}
void resizeMap(int ox, int oy, int nx, int ny, std::vector<char> &mapD)
{
    std::vector<char> oldtmp = mapD;
    mapD.clear();
    for (int aa = 0; aa < nx * ny; aa++)
        mapD.push_back(0);
    for (int y = 0; y < ny && y < oy; y++)
    {
        for (int x = 0; x < nx && x < ox; x++)
        {
            mapD[x + nx * y] = oldtmp[x + ox * y];
        }
    }
}
void resizeTiles(int x, std::vector<char> &mapD)
{
    mapD.clear();
    for (int aa = 0; aa < x; aa++)
        mapD.push_back(0);
}

int main()
{

    unsigned int mapW = 0;
    unsigned int mapH = 0;
    unsigned int sx = 0;
    unsigned int sy = 0;
    unsigned int usableTilesCount = 0;

    std::vector<char> mapData;
    std::string musicName = "\0\0\0\0\0\0\0\0";
    std::vector<char> linkData;
    std::string command = "";

    while (command != "quit")
    {
        bool getVal = false;
        std::cout << "Command:";
        std::cin >> command;

        if (command == "list")
        {
            std::cout << "MAPWIDTH:" << mapW << std::endl;
            std::cout << "MAPHEIGHT:" << mapH << std::endl;
            std::cout << "STARTX:" << sx << std::endl;
            std::cout << "STARTY:" << sy << std::endl;

            std::cout << "MUSICFILE:" << musicName << std::endl;
            std::cout << "LINKDATA:" << std::endl;
            for (int i = 0; i < linkData.size(); i++)
            {
                printf("%02X ", linkData[i]);
            }
            std::cout << std::endl;

            std::cout << "MAPDATA:" << std::endl;
            for (int i = 0; i < mapData.size(); i += mapW)
            {
                for (int j = 0; j < mapW; j++)
                {
                    printf("%02X ", mapData[i + j]);
                }
                std::cout << std::endl;
            }
        }
        else if (command == "music")
        {
            std::string tmpname = "\0\0\0\0\0\0\0\0";
            std::cin >> tmpname;
            musicName = tmpname.substr(0, 8);
        }
        else if (command == "setw")
        {
            int oldW = mapW;
            std::cin >> mapW;
            resizeMap(oldW, mapH, mapW, mapH, mapData);
        }
        else if (command == "seth")
        {
            int oldH = mapH;
            std::cin >> mapH;
            resizeMap(mapW, oldH, mapW, mapH, mapData);
        }
        else if (command == "setlinks")
        {
            std::cin >> usableTilesCount;
            resizeMap(linkData.size(), 1, usableTilesCount, 1, linkData);
            //resizeTiles(usableTilesCount, linkData);
        }
        else if (command == "link")
        {
            int modt;
            int tmp = 0;
            std::cin >> modt;
            if (modt < usableTilesCount)
            {
                std::cin >> std::hex >> tmp;
                linkData[modt] = tmp;
            }
        }
        else if (command == "tile")
        {
            unsigned int mody;
            unsigned int modx;
            int tmp = 0;
            std::cin >> modx;
            std::cin >> mody;
            if (modx < mapW && mody < mapH)
            {
                std::cin >> std::hex >> tmp;
                mapData[mapW * mody + modx] = tmp;
            }
        }
        else if (command == "setstart")
        {
            std::cin >> sx;
            std::cin >> sy;   
        }

        else if (command == "save")
        {
            std::cin >> command;
            std::ofstream mapf("./" + command,std::ofstream::binary);
            if (mapf.is_open())
            {
                mapf.write(musicName.c_str(), 8);
                int aaa = sx;
                mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                aaa = sy;
                mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                mapf.put((mapW >> 8) & 0xFF);
                mapf.put((mapW & 0xFF));
                mapf.put((mapH >> 8) & 0xFF);
                mapf.put((mapH & 0xFF));
                mapf.put(usableTilesCount & 0xFF);
                for (char a : linkData)
                {
                    mapf.put(a);
                }
                for (char a : mapData)
                {
                    mapf.put(a);
                }
                mapf.close();
            }
        }
        else if (command == "load")
        {
            std::cin >> command;
            std::ifstream mapf("./" + command);
            char tmpchar;
            if (mapf.is_open())
            {
                musicName = "";
                for (int i = 0; i < 8; i++)
                {
                    mapf.read(&tmpchar, 1);
                    musicName += tmpchar;
                }

                mapf.read((char *)&sx, 4);
                mapf.read((char *)&sy, 4);

                mapf.read(&tmpchar, 1);
                mapW = tmpchar << 8;
                mapf.read(&tmpchar, 1);
                mapW = mapW | tmpchar;

                mapf.read(&tmpchar, 1);
                mapH = tmpchar << 8;
                mapf.read(&tmpchar, 1);
                mapH = mapH | tmpchar;
                usableTilesCount = 0;
                mapf.read(&tmpchar, 1);
                usableTilesCount = tmpchar;

                linkData.clear();
                for (int i = 0; i < usableTilesCount; i++)
                {
                    mapf.read(&tmpchar, 1);
                    linkData.push_back(tmpchar);
                }

                mapData.clear();
                for (int i = 0; i < mapW * mapH; i++)
                {
                    mapf.read(&tmpchar, 1);
                    mapData.push_back(tmpchar);
                }

                mapf.close();
            }
        }
        else if (command == "new")
        {
            mapW = 0;
            mapH = 0;
            sx = 0;
            sy = 0;
            usableTilesCount = 0;
            mapData.clear();
            musicName = "\0\0\0\0\0\0\0\0";
            linkData.clear();
        }
        else if (command == "clearmap")
        {
            resetMap(mapW, mapH, mapData);
        }
    }
}