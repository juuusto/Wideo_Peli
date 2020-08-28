class Player
{
public:
    Player(std::string name, std::string image, Vehicle car, int x = 0, int y = 0, int ammo = 20, int hp = 6) : x_(0), y_(0), name_(name), image_(image), car_(car), ammo_(ammo), hp_(hp){};
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
    void setName(std::string name)
    {
        name_ =name;
    };
    Vehicle getCar()
    {
        return car_;
    };
    void setCar(Vehicle car)
    {
         car_=car;
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

protected:
    int x_;
    int y_;
    std::string name_;
    std::string image_;
    Vehicle car_;
    int ammo_;
    int hp_;
};