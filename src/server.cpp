#include <SFML/Network.hpp>
#include<iostream>





struct playerData{
    int x;
    int y;
    int r;
    int type;
};




int main()
{

    unsigned short inPort = 54010;
    unsigned short outPort = 54000;
    sf::UdpSocket socket;
    socket.bind(inPort) != sf::Socket::Done;
    socket.setBlocking(false);
    std::vector<playerData> players;
    while(true){
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
            std::cout << "Connection... \n"<<std::endl;
            players.push_back(playerData{100,100,0,(int)players.size()});
            res = "OK;"+std::to_string(players.size()-1)+";";
        } else if (tmp_arr[0]=="UPDATE"){
            std::cout << "Update... \n"<<std::endl;
            players[stoi(tmp_arr[4])]={stoi(tmp_arr[1]),stoi(tmp_arr[2]),stoi(tmp_arr[3]),stoi(tmp_arr[4])};
            res = "";
            for (playerData pd: players){
                res += std::to_string(pd.x)+";"+std::to_string(pd.y)+";"+std::to_string(pd.r)+";"+std::to_string(pd.type)+";";
            }


        } 
        std::cout << res <<"\n"<<std::endl;
        if (socket.send(res.c_str(), res.size()+1, sender, outPort) != sf::Socket::Done)
        {
            // error...
        }

    }
}