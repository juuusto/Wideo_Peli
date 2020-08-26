class Player
{
public:
    Player(std::string name, std::string image, Vehicle car, int x = 0, int y = 0, int ammo = 20, int hp = 6, int laps = 1) : x_(0), y_(0), name_(name), image_(image), car_(car), ammo_(ammo), hp_(hp), laps_(laps),checkpoint1_(false),checkpoint2_(false){};
    ~Player()
    {
    }
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
    int getAmmo()
    {
        return ammo_;
    }
    int getHp()
    {
        return hp_;
    }

    void shoot()
    {
        ammo_--;
    }
    void hit()
    {
        hp_--;
    }
    void incrementLap(){
        laps_++;
    }
    int getLaps(){
        return laps_;
    }
    void trueCheckpoint1(){
        checkpoint1_ = true;
    }
    void falseCheckpoint1(){
        checkpoint1_ = false;
    }
    void trueCheckpoint2(){
        checkpoint2_ = true;
    }
    void falseCheckpoint2(){
        checkpoint2_ = false;
    }
    bool getCheckpoint1(){
        return checkpoint1_;
    }
    bool getCheckpoint2(){
        return checkpoint2_;
    }

protected:
    int x_;
    int y_;
    std::string name_;
    std::string image_;
    Vehicle car_;
    int ammo_;
    int hp_;
    int laps_;
    bool checkpoint1_;
    bool checkpoint2_;
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