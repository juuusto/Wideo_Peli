#include <SFML/Network.hpp>
#include<iostream>
#include <ctime>




struct playerServerData{
    int x;
    int y;
    int r;
    int type;
    std::time_t connectTimeout;
    std::string name;
};
struct projectileData{
    int x;
    int y;
};



int main()
{
    std::string SERVER_MAP = "map3.map";
    unsigned short inPort = 54010;
    unsigned short outPort = 54000;
    sf::UdpSocket socket;
    socket.bind(inPort) != sf::Socket::Done;
    socket.setBlocking(false);
    std::vector<playerServerData> players;
    std::vector<std::vector<projectileData>> projData;


    std::time_t currentTime = std::time(nullptr);
    while(true){
        currentTime = std::time(nullptr);

        sf::IpAddress sender;
        char data[100];
        std::size_t received;
        unsigned short port;
        if (socket.receive(data, 100, received, sender, port) != sf::Socket::Done)
        {
            // error...
        }
        
        std::string s(data);

        if(received == 0) continue;
        //std::cout << s<<std::endl;
        std::string delimiter = ";";
        size_t pos = 0;
        int i = 0;
        std::string command;
        std::vector<std::string> tmp_arr;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            command = s.substr(0, pos);
            //std::cout << command << std::endl;
            tmp_arr.push_back(command);
            s.erase(0, pos + delimiter.length());
            i+=1;
        }
        //std::cout <<"\n" << tmp_arr[0] <<"\n"<<std::endl;
        std::string res;
        if(tmp_arr[0]=="CONNECT"){
            std::cout << "Connection... \n"<< std::to_string(players.size())<<std::endl;
            int pCount = 0;
            for (playerServerData pd: players){
                if(currentTime-pd.connectTimeout<2)pCount +=1;
            }
            
            if(pCount == 0){
                players.clear();
            }

            if (pCount < 15){
                projData.push_back(std::vector<projectileData>());
                players.push_back(playerServerData{100,100,0,(int)players.size(),currentTime,tmp_arr[1]});
                res = "OK;"+std::to_string(players.size()-1)+";"+SERVER_MAP+";";
            } else {
                res = "ERROR;TOO MANY PLAYERS;";
            }

        } else if (tmp_arr[0]=="UPDATE"){
            //std::cout << "Update... \n"<<std::endl;
            players[stoi(tmp_arr[4])]={stoi(tmp_arr[1]),stoi(tmp_arr[2]),stoi(tmp_arr[3]),stoi(tmp_arr[4]),currentTime,players[stoi(tmp_arr[4])].name};
            res = "";
            for (playerServerData pd: players){
                if(currentTime-pd.connectTimeout<2)res += std::to_string(pd.x)+";"+std::to_string(pd.y)+";"+std::to_string(pd.r)+";"+std::to_string(pd.type)+";"+pd.name+";";
            }
        } else if (tmp_arr[0]=="UASSETS"){
            std::cout << "Update projectiles... \n"<<std::endl;


            projData[stoi(tmp_arr[1])].clear();
            for(int pri = 2; pri<tmp_arr.size();pri+=2){
                projData[stoi(tmp_arr[1])].push_back({stoi(tmp_arr[pri]),stoi(tmp_arr[pri+1])});
            }

            res = "";
            for (std::vector<projectileData> pr: projData){
                for (projectileData pr1: pr){
                    std::cout << std::to_string(pr1.x)+";"+std::to_string(pr1.y)+";"; 
                    res += std::to_string(pr1.x)+";"+std::to_string(pr1.y)+";";
                }
            }
            std::cout <<std::endl;
        } else if (tmp_arr[0]=="PLAYERS"){
            //std::cout << "Sending Player list... \n"<<std::endl;
            res = "";
            int pCount = 0;
            for (playerServerData pd: players){
                if(currentTime-pd.connectTimeout<2)pCount +=1;
            }

            res += "PLAYERCOUNT;"+std::to_string(pCount)+";";
        } 
        //std::cout << res <<"\n"<<std::endl;
        if (socket.send(res.c_str(), res.size()+1, sender, outPort) != sf::Socket::Done)
        {
            // error...
        }
//std::cout << "Time: "<< currentTime <<std::endl;
    }
}
