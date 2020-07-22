class Player
{
public:
    Player(std::string name, std::string image, Vehicle car, int x = 0, int y = 0) : x_(0), y_(0), name_(name), image_(image), car_(car){};
    std::pair<int, int> getLocation()
    {
        return std::pair<int, int>(x_, y_);
    };
    void updateLocation()
    {
        return;
    }
    std::string getName()
    {
        return name_;
    };
    Vehicle getCar()
    {
        return car_;
    };

protected:
    int x_;
    int y_;
    std::string name_;
    std::string image_;
    Vehicle car_;

};

class aiPlayer : public Player
{
public:
    aiPlayer(std::string name, std::string image, Vehicle car, int x = 0, int y = 0) : Player(name, image, car, x, y){};
    void updateLocation()
    {
        return;
    }
};
class netPlayer : public Player
{
public:
    netPlayer(std::string name, std::string image, Vehicle car, int x = 0, int y = 0) : Player(name, image, car, x, y){};
    void updateLocation()
    {
        return;
    }
};

class otherPlayers
{
public:
    otherPlayers(){};
    std::pair<int, int> getPosition(int id)
    {
        return players_[id].getLocation();
    };
    int getOtherPlayerCount()
    {
        return players_.size();
    }
    void addPlayer(Player player)
    {
        players_.push_back(player);
    };
    void updatePlayers()
    {
        for (auto player : players_)
        {
            player.updateLocation();
        }
    }

private:
    std::vector<Player> players_;
};