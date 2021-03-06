#include <SFML/Network.hpp>
#include <iostream>
#include "network.hpp"

// parse ; seperated values to vector from string
std::vector<std::string> Network::parseData(std::string data)
{
    std::vector<std::string> retVec;
    size_t pos = 0;
    std::string command;
    while ((pos = data.find(";")) != std::string::npos)
    {
        command = data.substr(0, pos);
        //std::cout << command << std::endl;
        retVec.push_back(command);
        data.erase(0, pos + 1);
    }
    return retVec;
}

// connect the player
bool Network::connect(Player player)
{
    socket_.setBlocking(true);
    std::string data2 = "CONNECT;" + player.getName() + ";";
    sf::IpAddress recipient = serveraddr_;
    if (socket_.send(data2.c_str(), data2.size() + 1, recipient, outPort_) != sf::Socket::Done)
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
    std::vector<std::string> tmpData = this->parseData(s);
    // save the values and id given by server
    if (tmpData.size() == 4 && tmpData[0] == "OK")
    {
        conId_ = std::stoi(tmpData[1]);
        srvMap_ = tmpData[2];
        connected_ = true;
        srvLapCount_ = std::stoi(tmpData[3]);
    }
    socket_.setBlocking(false);
    return connected_;
}

// set connection address
void Network::setAddress(std::string address)
{
    serveraddr_=address;
}

// get player id
int Network::getConId()
{
    return conId_;
}
bool Network::disconnect()
{
    return !connected_;
}

// get connection status
bool Network::isConnected()
{
    return connected_;
}

// send and recieve player locations
void Network::refreshData(playerData pd)
{
    socket_.setBlocking(true);
    sf::IpAddress recipient = serveraddr_;

    std::string data2 = "UPDATE;" + std::to_string(pd.x) + ";" + std::to_string(pd.y) + ";" + std::to_string(pd.r) + ";" + std::to_string(pd.type) + ";";
    if (socket_.send(data2.c_str(), data2.size() + 1, recipient, outPort_) != sf::Socket::Done)
    {
        // error...
    }

    sf::IpAddress sender;
    char data[1300];
    std::size_t received;
    unsigned short port;
    if (socket_.receive(data, 1300, received, sender, port) != sf::Socket::Done)
    {
        // error...
    }

    if (received == 0)
        return;
    std::string s(data);
    // parse data from other players
    try
    {
        std::vector<std::string> tmpData = this->parseData(s);
        if (tmpData.size() > 0)
            data_.clear();
        for (int j = 0; j < tmpData.size(); j += 5)
        {
            data_.push_back({std::stoi(tmpData[j]), std::stoi(tmpData[j + 1]), std::stoi(tmpData[j + 2]), std::stoi(tmpData[j + 3]), tmpData[j + 4]});
        }
    }
    catch (const std::exception &e)
    {
    }
    socket_.setBlocking(false);
}
// update projectile locations
void Network::refreshAssetData(std::vector<Projectile> pr)
{
    socket_.setBlocking(true);
    sf::IpAddress recipient = serveraddr_;
    std::string data2 = "UASSETS;" + std::to_string(conId_) + ";";
    for (Projectile aa : pr)
    {
        data2 += std::to_string(aa.sprite_.getPosition().x) + ";" + std::to_string(aa.sprite_.getPosition().y) + ";";
    }
    //+ std::to_string(pd.x) + ";" + std::to_string(pd.y) + ";" + std::to_string(pd.r) + ";" + std::to_string(pd.type) + ";";
    if (socket_.send(data2.c_str(), data2.size() + 1, recipient, outPort_) != sf::Socket::Done)
    {
        // error...
    }

    sf::IpAddress sender;
    char data[300];
    std::size_t received;
    unsigned short port;
    if (socket_.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        // error...
    }

    if (received == 0)
        return;
    std::string s(data);
    // parse received projectiles
    try
    {
        std::vector<std::string> tmpData = this->parseData(s);
        if (projdata_.size() > 0)
            projdata_.clear();
        for (int j = 0; j < tmpData.size(); j += 2)
        {
            projdata_.push_back(std::pair<int, int>(std::stoi(tmpData[j]), std::stoi(tmpData[j + 1])));
        }
    }
    catch (const std::exception &e)
    {
    }
    socket_.setBlocking(false);
}

// get data for spesific player
playerData Network::getPlayerData(int id)
{
    return data_[id];
}
std::vector<playerData> Network::getPlayerDataAll()
{
    return data_;
}
std::vector<std::pair<int, int>> Network::getProjectileDataAll()
{
    return projdata_;
}

int Network::getPlayerCount()
{
    return data_.size();
}
int Network::getLapCount()
{
    return srvLapCount_;
}
std::string Network::getServerMap()
{
    return srvMap_;
}

// declare to the server that you won :^)
bool Network::iWin()
{
    socket_.setBlocking(true);
    sf::IpAddress recipient = serveraddr_;
    std::string data2 = "IWIN;" + std::to_string(conId_) + ";";

    if (socket_.send(data2.c_str(), data2.size() + 1, recipient, outPort_) != sf::Socket::Done)
    {
        // error...
    }

    sf::IpAddress sender;
    char data[300];
    std::size_t received;
    unsigned short port;
    if (socket_.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        // error...
    }

    if (received == 0)
        return false;
    std::string s(data);
    socket_.setBlocking(false);
    try
    {
        std::vector<std::string> tmpData = this->parseData(s);
        return tmpData[0]=="OK";
    }
    catch (const std::exception &e)
    {
    }
    
    return false;
}

// get status of game
int Network::gameStatus()
{
    socket_.setBlocking(true);
    sf::IpAddress recipient = serveraddr_;
    std::string data2 = "GAMESTATUS;" + std::to_string(conId_) + ";";

    if (socket_.send(data2.c_str(), data2.size() + 1, recipient, outPort_) != sf::Socket::Done)
    {
        // error...
    }

    sf::IpAddress sender;
    char data[300];
    std::size_t received;
    unsigned short port;
    if (socket_.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        // error...
    }

    if (received == 0)
        return false;
    std::string s(data);
    socket_.setBlocking(false);
    try
    {
        std::vector<std::string> tmpData = this->parseData(s);
        if( tmpData[0]=="KEEPGOING") return 1;
        else if( tmpData[0]=="HOLD") return 2;
        else return 0;
    }
    catch (const std::exception &e)
    {
    }
    
    return 0;
}