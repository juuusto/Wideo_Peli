#include <SFML/Network.hpp>
#include <iostream>
#include "network.hpp"



bool Network::connect()
{
    socket_.setBlocking(true);
    std::string data2 = "CONNECT;";
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

    if (s.substr(0, 3) == "OK;")
    {
        s.erase(0, 3);
        int pos = 0;
        if ((pos = s.find(";")) != std::string::npos)
        {
            conId_ = std::stoi(s.substr(0, pos));
            connected_ = true;
        }
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
    // UDP socket:
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
    std::string delimiter = ";";
    size_t pos = 0;
    int i = 0;
    std::string command;
    int tmp_arr[100];
    try
    {

        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            command = s.substr(0, pos);
            //std::cout << command << std::endl;
            tmp_arr[i] = std::stoi(command);
            s.erase(0, pos + delimiter.length());
            i += 1;
        }
        if (i != 0)
            data_.clear();
        for (int j = 0; j < i; j += 4)
        {
            data_.push_back({tmp_arr[j], tmp_arr[j + 1], tmp_arr[j + 2], tmp_arr[j + 3]});
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
