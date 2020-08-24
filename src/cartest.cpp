#include <SFML/Graphics.hpp>
#include <math.h>
#include "Map.cpp"
#include "vehicle.hpp"
#include "player.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>


// Test for the basic car movement and camera movement

class Game
{
public:
    Game(Map map, Player player) : map_(map), player_(player){

                                                                                    };

    ~Game(){

    } 
    int run(sf::RenderWindow& window, int windowX = 800, int windowY = 600)
    {
        int selectedType =0;         
        sf::View view(sf::FloatRect(0.f, 20.f, windowX, windowY - 20.f));


        std::vector<sf::Texture> textures;
        for(auto tx: map_.getTiles()){
            sf::Texture aaa;

            aaa.loadFromFile(tx.getTileName());
            textures.push_back(aaa);
        }


        window.setView(view);

        float currentSpeed = 0.f;

        sf::Texture car;
        if (!car.loadFromFile(player_.getCar().getSprite()))
        {
            return 0;
        }


        sf::Sprite playerSprite;


        playerSprite.move(windowX / 2, windowY / 2);
        playerSprite.setTexture(car);
        playerSprite.setOrigin(car.getSize().x * 0.5f, car.getSize().y * 0.5f);
        playerSprite.setPosition(playerSprite.getPosition().x + playerSprite.getOrigin().x, playerSprite.getPosition().y + playerSprite.getOrigin().y);
        float rot = playerSprite.getRotation();
        window.setFramerateLimit(60);


        while (window.isOpen())
        {

            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                currentSpeed += player_.getCar().getAcceleration();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                currentSpeed -= player_.getCar().getAcceleration();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                rot -= player_.getCar().getTurnSpeed();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                rot += player_.getCar().getTurnSpeed();

                
            currentSpeed -= player_.getCar().getDrag();
            if (currentSpeed < 0.f)
                currentSpeed = 0.f;
            else if (currentSpeed > player_.getCar().getMaxSpeed())
                currentSpeed = player_.getCar().getMaxSpeed();
            if (rot > 360.f)
                rot = 0.f;
            else if (rot < 0.f)
                rot = 360.f;
            if (rot - playerSprite.getRotation() != 0.f)
                playerSprite.rotate(rot - playerSprite.getRotation());

            float playerX = playerSprite.getPosition().x;
            float playerY = playerSprite.getPosition().y;
            int blockX = playerX / map_.getBlockSize();
            int blockY = playerY / map_.getBlockSize();

            int offsetInTileX = playerX-blockX*map_.getBlockSize();
            int offsetInTileY = playerY-blockY*map_.getBlockSize();


            float xChange = currentSpeed * cos((rot / 180.f) * 3.14f);
            float yChange = currentSpeed * sin((rot / 180.f) * 3.14f);


            if(playerX<0 || playerY<0 || blockX>=map_.getMapWidth() || blockY>=map_.getMapHeight()){
                xChange *=-3.f;
                yChange *=-3.f;
            } else if(map_.getTile(blockX,blockY).isCollision(offsetInTileX,offsetInTileY)){
                // if collision, dont allow move (reverse move)
                xChange *=-3.f;
                yChange *=-3.f;
            }


            playerSprite.move(xChange, yChange);
  
            
            
            view.move(xChange, yChange);
            window.setView(view);

            window.clear();


            int drawDistance = 3;
            for(int i=blockX-drawDistance; i<blockX+drawDistance;i++){
                for(int j=blockY-drawDistance; j<blockY+drawDistance;j++){
                    sf::Sprite drawTile;
                    if(i<0 || j<0 || i>=map_.getMapWidth() || j>=map_.getMapHeight())continue;

                    drawTile.setTexture( textures[map_.getTileId(i,j)]);
                    drawTile.move(i*map_.getBlockSize(),j*map_.getBlockSize());
                    window.draw(drawTile);

                } 
            }


            window.draw(playerSprite);
            window.display();
        }
        return 0;
    };

private:
    Map map_;
    Player player_;
};


int main() {
    int windowX = 800;
    int windowY = 600;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "WIDEO PELI");

    Map kartta("map1.map");

    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);

    Player pelaaja("TESTING CAR", "", ajoneuvo);

    Game peli(kartta, pelaaja);

    int res = peli.run(window,windowX, windowY);

    return res;
}