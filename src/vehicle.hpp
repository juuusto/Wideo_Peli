#include <string>

class Vehicle
{
public:
    Vehicle(std::string sprite, float max_speed, float acceleration, float turn_speed, float drag) : sprite_(sprite), max_speed_(max_speed), acceleration_(acceleration), turn_speed_(turn_speed), drag_(drag){};
    std::string getSprite()
    {
        return sprite_;
    }
    float getMaxSpeed()
    {
        return max_speed_;
    }
    float getAcceleration()
    {
        return acceleration_;
    }
    float getTurnSpeed()
    {
        return turn_speed_;
    }
    float getDrag()
    {
        return drag_;
    }

private:
    std::string sprite_;
    float max_speed_;
    float acceleration_;
    float turn_speed_;
    float drag_;
};

struct Vehicle1
{
    std::string sprite;
    float max_speed;
    float acceleration;
    float turn_speed;
    float drag;
};
