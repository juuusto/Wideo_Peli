
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Map.cpp"
#include "vehicle.hpp"
#include "player.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "network.hpp"



class Game
{
public:
    Game(Map map, Player player, otherPlayers others, Network* net) : map_(map), player_(player), others_(others),net_(net){

                                                                                    };

    ~Game(){

    } 
    int run(int windowX = 800, int windowY = 600)
    {
        int selectedType =0;         
        sf::RenderWindow window(sf::VideoMode(windowX, windowY), "WIDEO PELI");
        sf::View view(sf::FloatRect(0.f, 20.f, windowX, windowY - 20.f));


        std::vector<sf::Texture> textures;
        for(auto tx: map_.getTiles()){
            sf::Texture aaa;

            aaa.loadFromFile(tx.getTileName());
            textures.push_back(aaa);
        }

        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("assets/arial.ttf"))
        {
            return 0;
        }

        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(10, 20);

        window.setView(view);

        float currentSpeed = 0.f;

        
        sf::Texture car;
        if (!car.loadFromFile(player_.getCar().getSprite()))
        {
            return 0;
        }

        sf::Texture otherCar;
        if (!otherCar.loadFromFile("assets/carNET.png"))
        {
            return 0;
        }
       
        sf::Sprite playerSprite;
        sf::Sprite netSprite;
        netSprite.move(windowX / 2, windowY / 2);
        netSprite.setTexture(otherCar);
        netSprite.setOrigin(otherCar.getSize().x * 0.5f, otherCar.getSize().y * 0.5f);
        netSprite.setPosition(netSprite.getPosition().x + netSprite.getOrigin().x, netSprite.getPosition().y + netSprite.getOrigin().y);


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

            if(map_.getTile(blockX,blockY).isCollision(offsetInTileX,offsetInTileY)){
                // if collision, dont allow move (reverse move)

             xChange *=-3.f;
             yChange *=-3.f;


            }


            if (playerX < 0)
                xChange = 1.f;
            else if (playerX > map_.getMapWidth() * map_.getBlockSize())
                xChange = -1.f;
            if (playerY < 0)
                yChange = 1.f;
            else if (playerY > map_.getMapHeight() * map_.getBlockSize())
                yChange = -1.f;

            view.move(xChange, yChange);
            window.setView(view);
            text.setString(
                "PLAYING AS: " + player_.getName() + " BLOCK X:" + std::to_string(blockX) + " Y:" + std::to_string(blockY) + " Type:" + std::to_string(map_.getTileId(blockX,blockY)) + 
                "\nW:" + std::to_string(view.getSize().x)+" H:" + std::to_string(view.getSize().y)+" OffsetXinTile:"+std::to_string(offsetInTileX)+" OffsetYinTile:"+std::to_string(offsetInTileY)
            );

            text.move(xChange, yChange);
            playerSprite.move(xChange, yChange);


            // do this completely
            others_.updatePlayers();
            //window.getPosition();

            window.clear();



            for(int i=blockX-3; i<blockX+3;i++){
                for(int j=blockY-3; j<blockY+3;j++){
                    sf::Sprite drawTile;
                    if(i<0 || j<0 || i>=map_.getMapWidth() || j>=map_.getMapHeight())continue;

                    drawTile.setTexture( textures[map_.getTileId(i,j)]);
                    drawTile.move(i*map_.getBlockSize(),j*map_.getBlockSize());
                    window.draw(drawTile);

                } 
            }

            if(net_->isConnected()){
                net_->refreshData(playerData{(int)playerX,(int)playerY,(int)rot,net_->getConId()});
                
                for(playerData pd :net_->getPlayerDataAll()){
                    //std::cout<<std::to_string(pd.x)<< ";"<<std::to_string(pd.y)<<";"<<std::to_string(pd.r)<<std::endl;
                    netSprite.setPosition(pd.x,pd.y);
                    netSprite.setRotation(pd.r);
                    if(pd.type!=net_->getConId())window.draw(netSprite);
                }

            }




            window.draw(playerSprite);
            window.draw(text);
            window.display();
        }
        return 0;
    };

private:
    Map map_;
    Player player_;
    otherPlayers others_;
    Network* net_;
};

int main()
{
    std::string addr="";
    std::cout<<"IP:";
    std::cin >> addr;

    Network aa(addr);
    if(addr!="n") aa.connect();


    Network *verkko = &aa;


    std::vector<Tile> tileArr;
    tileArr.push_back(Tile("assets/tile.png"));
    tileArr.push_back(Tile("assets/tile1.png"));
    std::vector<int> idArray = {1,1,1,1,1,1,1};
//return 0;
    Map kartta(tileArr,200,14,14,idArray);
    kartta.setTile(0,0,1);
    //kartta.constructMap();
        //

    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);
    otherPlayers muutPelaajat;

    Player pelaaja("Huutis Ukko", "", ajoneuvo);

    Game peli(kartta, pelaaja, muutPelaajat, verkko);
    
    int res = peli.run(800, 600);
    tileArr.clear();

    return res;
}
