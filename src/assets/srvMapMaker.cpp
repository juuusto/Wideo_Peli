#include <iostream>
#include <vector>
#include <fstream>

int main()
{

    std::vector<std::vector<std::pair<int, int>>> aiData;
    int aiCount = 0;
    int thisAi = 0;
    std::string clientMap;
    std::string command = "";

    while (command != "quit")
    {
        bool getVal = false;
        std::cout << "Command:";
        std::cin >> command;

        if (command == "list")
        {
            std::cout << "ClientMap:" << clientMap << std::endl;
            std::cout << "AI's:" << aiCount << std::endl;
            std::cout << "Selected AI:" << thisAi << std::endl;
            std::cout << "Current path:" << std::endl;
            if(aiCount>0 ){
                for(int aa = 0; aa< aiData[thisAi].size(); aa++){
                    std::cout << "ID:" << aa << " X:"<< aiData[thisAi][aa].first << " Y:" << aiData[thisAi][aa].second<< std::endl; 
                }
            }

           
        }
        else if (command == "addai")
        {

            //std::cin >> command;
            aiData.push_back(std::vector<std::pair<int, int>>{});
            aiCount++;
            thisAi = aiCount-1;
        }
        else if (command == "map")
        {
            std::cin >> command;
            clientMap = command;
        }
        else if (command == "useai")
        {
            int id=0;
            std::cin >> id;
            if(id<aiCount) thisAi=id;

        }
        else if (command == "removeai")
        {
            std::cin >> command;
            aiData.erase(aiData.begin()+std::stoi(command));
            thisAi =0;
            aiCount--;
        }
        else if (command == "addc")
        {
            int x=0;
            int y=0;
            std::cin >> x;
            std::cin >> y;
            if(aiCount>0)aiData[thisAi].push_back(std::pair<int,int>(x,y));
        }
        else if (command == "insertc")
        {
            int id=0;
            std::cin >> id;
            int x=0;
            int y=0;
            std::cin >> x;
            std::cin >> y;
            if(aiCount>0 && aiData[thisAi].size()>id)aiData[thisAi].insert(aiData[thisAi].begin()+id,std::pair<int,int>(x,y));
        }
        else if (command == "modc")
        {
            int id=0;
            std::cin >> id;
            int x=0;
            int y=0;
            std::cin >> x;
            std::cin >> y;
            if(aiCount>0 && aiData[thisAi].size()>id)aiData[thisAi][id]=std::pair<int,int>(x,y);
        }
        else if (command == "removec")
        {
            int id=0;
            std::cin >> id;
            aiData[thisAi].erase(aiData[thisAi].begin()+id);
        }
        else if (command == "removecl")
        {
            if(aiData[thisAi].size()>0)aiData[thisAi].erase(aiData[thisAi].end()-1);
        }

        else if (command == "save")
        {
            std::cin >> command;
            std::ofstream mapf("./" + command,std::ofstream::binary);
            int aaa = 0;
            if (mapf.is_open())
            {
                mapf.write(clientMap.c_str(), clientMap.size());
                char tmpchar = 0;
                mapf.write(&tmpchar,1);
                for(int aa=0; aa< aiData.size();aa++){
                    for(auto bb: aiData[aa]){
                        aaa = bb.first;
                        mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                        aaa = bb.second;
                        mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                    }
                    if(aa < aiData.size()-1){
                        aaa = 0;
                        mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                        mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                    }

                }
                aaa = 0xFFFFFFFF;
                mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                mapf.write(reinterpret_cast<const char *>(&aaa), 4);
                mapf.close();
            }
        }
        else if (command == "load")
        {
            std::cin >> command;
            std::ifstream mapf("./"+command);
            if (mapf.is_open())
            {

                clientMap = "";
                char tmpchar = 0xFF;
                while (true)
                {
                    mapf.read(&tmpchar, 1);
                    if (tmpchar == 0)
                        break;
                    clientMap += tmpchar;
                }
                bool readMore = true;

                while (readMore)
                {
                    std::vector<std::pair<int, int>> tmpData{};
                    while (true)
                    {
                        int x = 0;
                        int y = 0;
                        mapf.read((char *)&x, 4);
                        mapf.read((char *)&y, 4);
                        if (x == 0 && y == 0)
                            break;
                        else if (x == 0xFFFFFFFF && y == 0xFFFFFFFF)
                        {
                            readMore = false;
                            break;
                        }
                        tmpData.push_back(std::pair<int, int>(x, y));
                    }
                    aiData.push_back(tmpData);
                }
                aiCount = aiData.size();
                thisAi=0;
                mapf.close();
            }
        }
        else if (command == "new")
        {
            aiData.clear();
            aiCount = 0;
            thisAi = 0;
            clientMap ="";
            command = "";
        }
    }
}