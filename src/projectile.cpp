
class Projectile
{
public:
	sf::Sprite sprite_;
	sf::Vector2f speed_;
	float maxSpeed_;

	Projectile(float radius = 5.f)
		: speed_(0.f, 0.f), maxSpeed_(20.f)
	{
	
		
	}
};
