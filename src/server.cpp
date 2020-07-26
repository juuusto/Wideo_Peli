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




int main()
{

    unsigned short inPort = 54010;
    unsigned short outPort = 54000;
    sf::UdpSocket socket;
    socket.bind(inPort) != sf::Socket::Done;
    socket.setBlocking(false);
    std::vector<playerServerData> players;
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
        std::cout << s<<std::endl;
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
        std::cout <<"\n" << tmp_arr[0] <<"\n"<<std::endl;
        std::string res;
        if(tmp_arr[0]=="CONNECT"){
            std::cout << "Connection... \n"<< std::to_string(players.size())<<std::endl;
            if (players.size() < 15){
                players.push_back(playerServerData{100,100,0,(int)players.size(),currentTime,tmp_arr[1]});
                res = "OK;"+std::to_string(players.size()-1)+";";
            } else {
                res = "ERROR;TOO MANY PLAYERS;";
            }

        } else if (tmp_arr[0]=="UPDATE"){
            std::cout << "Update... \n"<<std::endl;
            players[stoi(tmp_arr[4])]={stoi(tmp_arr[1]),stoi(tmp_arr[2]),stoi(tmp_arr[3]),stoi(tmp_arr[4]),currentTime,players[stoi(tmp_arr[4])].name};
            res = "";
            for (playerServerData pd: players){
                if(currentTime-pd.connectTimeout<2)res += std::to_string(pd.x)+";"+std::to_string(pd.y)+";"+std::to_string(pd.r)+";"+std::to_string(pd.type)+";"+pd.name+";";
            }
        } else if (tmp_arr[0]=="PLAYERS"){
            std::cout << "Sending Player list... \n"<<std::endl;
            res = "";
            int pCount = 0;
            for (playerServerData pd: players){
                if(currentTime-pd.connectTimeout<2)pCount +=1;
            }

            res += "PLAYERCOUNT;"+std::to_string(pCount)+";";
        } 
        std::cout << res <<"\n"<<std::endl;
        if (socket.send(res.c_str(), res.size()+1, sender, outPort) != sf::Socket::Done)
        {
            // error...
        }
std::cout << "Time: "<< currentTime <<std::endl;
    }
}
