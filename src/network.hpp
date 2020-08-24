#include <SFML/Network.hpp>
#include<iostream>

struct playerData{
    int x;
    int y;
    int r;
    int type;
    std::string name;
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
    ~Network(){
        data_.clear();
        socket_.unbind();
    }
    std::vector<std::string> parseData(std::string data);
    bool connect(Player player);
    int getConId();
    bool disconnect();
    bool isConnected();
    void refreshData(playerData pd);
    void refreshAssetData(std::vector<Projectile> pr);
    playerData getPlayerData(int id);
    std::vector<playerData> getPlayerDataAll();
    int getPlayerCount();
    private:
        std::string serveraddr_;
        unsigned short inPort_;
        unsigned short outPort_;
        bool connected_;
        sf::UdpSocket socket_;
        std::vector<playerData> data_;
        std::vector<std::pair<int,int>> projdata_;
        int conId_;
};