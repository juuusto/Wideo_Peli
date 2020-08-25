#include <SFML/Graphics.hpp>

class Boost
{
public:
	sf::Sprite sprite_;
	sf::Texture boostText;

	Boost(){
		boostText.loadFromFile("assets/boost.png");
		sprite_.setTexture(boostText);
        
	}
};