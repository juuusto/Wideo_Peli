#include <SFML/Graphics.hpp>

class Tar
{
public:
	sf::Sprite sprite_;
	sf::Texture tarText;

	Tar(){
        tarText.loadFromFile("assets/tar.png");
		sprite_.setTexture(tarText);
        
	}
};