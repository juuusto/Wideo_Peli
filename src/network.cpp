#include <SFML/Network.hpp>
#include <iostream>
#include "network.hpp"

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
    if (tmpData.size() == 3 && tmpData[0] == "OK")
    {
        conId_ = std::stoi(tmpData[1]);
        srvMap_ = tmpData[2];
        connected_ = true;
    }
    socket_.setBlocking(false);
    return connected_;
}
int Network::getConId()
{
    return conId_;
}
bool Network::disconnect()
{
    return !connected_;
}
bool Network::isConnected()
{
    return connected_;
}
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

    try
    {
        std::vector<std::string> tmpData = this->parseData(s);
        if (tmpData.size() > 0)
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
std::string Network::getServerMap()
{
    return srvMap_;
}