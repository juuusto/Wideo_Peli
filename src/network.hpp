#include <SFML/Network.hpp>
#include<iostream>

struct playerData{
    int x;
    int y;
    int r;
    int type;
};
class Network {
    public:
    Network(std::string serveraddr="127.0.0.1", unsigned short inPort=54000, unsigned short outPort=54010):serveraddr_(serveraddr),inPort_(inPort),outPort_(outPort),connected_(false){
        // bind the socket to a port
        if (socket_.bind(inPort) != sf::Socket::Done)
        {
            // error...
        }
        socket_.setBlocking(false);
    }

    bool connect(){
        socket_.setBlocking(true);
        std::string data2 = "CONNECT;";
        sf::IpAddress recipient = serveraddr_;
        if (socket_.send(data2.c_str(), data2.size()+1, recipient, outPort_) != sf::Socket::Done)
        {
            // error...
        }

        sf::IpAddress sender;
        char data[100];
        std::size_t received;
        unsigned short port;
        if (socket_.receive(data, 100, received, sender, port) != sf::Socket::Done)
        {
            // error...
        }
        std::string s(data);

        if(s.substr(0,3)=="OK;"){
            conId_ = std::stoi(s.substr(3,1));
            connected_=true;
        }
        socket_.setBlocking(false);
        return connected_;
    }
    int getConId(){
        return conId_;
    }
    bool disconnect(){
        return !connected_;
    }
    bool isConnected(){
        return connected_;
    }
    void refreshData(playerData pd){
        // UDP socket:

        sf::IpAddress recipient = serveraddr_;

        std::string data2 ="UPDATE;"+std::to_string(pd.x)+";"+std::to_string(pd.y)+";"+std::to_string(pd.r)+";"+std::to_string(pd.type)+";";
        if (socket_.send(data2.c_str(), data2.size()+1, recipient, outPort_) != sf::Socket::Done)
        {
            // error...
        }

        sf::IpAddress sender;
        char data[100];
        std::size_t received;
        unsigned short port;
        if (socket_.receive(data, 100, received, sender, port) != sf::Socket::Done)
        {
            // error...
        }

        std::string s(data);
        if(s=="")return;
        std::string delimiter = ";";
        size_t pos = 0;
        int i = 0;
        std::string command;
        int tmp_arr[20];
        while ((pos = s.find(delimiter)) != std::string::npos) {
            command = s.substr(0, pos);
            //std::cout << command << std::endl;
            tmp_arr[i]=std::stoi(command);
            s.erase(0, pos + delimiter.length());
            i+=1;
        }
        if(i!=0) data_.clear();
        for(int j = 0; j<i;j+=4){
            data_.push_back({tmp_arr[j],tmp_arr[j+1],tmp_arr[j+2],tmp_arr[j+3]});
        }


    }
    playerData getPlayerData(int id){
        return data_[id];

    }
    std::vector<playerData> getPlayerDataAll(){
        return data_;

    }
    private:
        std::string serveraddr_;
        unsigned short inPort_;
        unsigned short outPort_;
        bool connected_;
        sf::UdpSocket socket_;
        std::vector<playerData> data_;
        int conId_;
};