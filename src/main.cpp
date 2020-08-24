
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include "Map.cpp"
#include "vehicle.hpp"
#include "player.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "projectile.hpp"
#include "network.cpp"




class Game
{
public:
    Game(Map map, Player player, Network* net) : map_(map), player_(player), net_(net){

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

        sf::Text UItext;
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
        UItext.setFont(font);
        UItext.setCharacterSize(24);
        UItext.setFillColor(sf::Color::Green);
        UItext.setPosition(10, 560);


        sf::Text nameTag;
        nameTag.setFont(font);
        nameTag.setCharacterSize(16);
        nameTag.setFillColor(sf::Color::White);

        window.setView(view);

        float currentSpeed = 0.f;

        
        // loading textures (implement textureloader class?)
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
       
        sf::Texture projTexture;
        if(!projTexture.loadFromFile("assets/projectile.png")){
            return 0;
        }
        
        sf::SoundBuffer shootingBuff;
        if(!shootingBuff.loadFromFile("assets/shoot.wav")){
            return 0;
        }

        sf::Sound shootingSound;
        shootingSound.setBuffer(shootingBuff);
        shootingSound.setVolume(10.f);
       


        sf::Sprite playerSprite;
        sf::Sprite netSprite;
        sf::Sprite netprojSprite;
        netprojSprite.setTexture(projTexture); 
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

        //timer for shooting
        shootingClock = sf::Clock();

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


            //shooting variables
            Projectile projectile;
            projectile.sprite_.setTexture(projTexture); 
            sf::Vector2f aimDirection;
            sf::Vector2f aimDirectionNorm;
            sf::Vector2f playerSpriteCenter;
            
            

            playerSpriteCenter = sf::Vector2f(playerSprite.getPosition().x , playerSprite.getPosition().y);
            aimDirection = sf::Vector2f( cos(playerSprite.getRotation() * (3.14/180)), sin(playerSprite.getRotation() * (3.14/180) ));
		    aimDirectionNorm = aimDirection / sqrt( (aimDirection.x * aimDirection.x ) + (aimDirection.y * aimDirection.y ) );


            //shooting
     

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player_.getAmmo() > 0 && shootingClock.getElapsedTime().asSeconds() > 2 ){
                shootingClock.restart();
                player_.shoot();
                shootingSound.play();
                projectile.sprite_.setPosition(playerSpriteCenter);
			    projectile.speed_ = aimDirectionNorm * projectile.maxSpeed_;
			    projectiles_.push_back(Projectile(projectile));
            }


            //erasing when projectiles are out of map
            for(size_t i = 0; i < projectiles_.size(); i++){
                projectiles_[i].sprite_.move(projectiles_[i].speed_);
                
                if(projectiles_[i].sprite_.getPosition().x < 0 || projectiles_[i].sprite_.getPosition().x > 5000 ||
                   projectiles_[i].sprite_.getPosition().y < 0 || projectiles_[i].sprite_.getPosition().y > 5000 )
                   {
                       projectiles_.erase(projectiles_.begin() + i);
                   }

            }




            //checking for other players projectiles hitting you
            for(size_t i = 0; i < netProjectiles_.size(); i++){
                if(netProjectiles_[i].sprite_.getGlobalBounds().intersects(playerSprite.getGlobalBounds())){
                    player_.hit();
                    netProjectiles_.erase(netProjectiles_.begin() + i);
                }

            }



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


            text.setString(
                "PLAYING AS: " + player_.getName() + " BLOCK X:" + std::to_string(blockX) + " Y:" + std::to_string(blockY) + " Type:" + std::to_string(map_.getTileId(blockX,blockY)) +                
                "\nW:" + std::to_string(view.getSize().x)+" H:" + std::to_string(view.getSize().y)+" OffsetXinTile:"+std::to_string(offsetInTileX)+" OffsetYinTile:"+std::to_string(offsetInTileY)  
            );


            UItext.setString(
                "Ammo:" + std::to_string(player_.getAmmo()) + "     CD: "+ std::to_string(shootingClock.getElapsedTime().asSeconds()) + "       HP: " + std::to_string(player_.getHp()) 
            );

        

            playerSprite.move(xChange, yChange);
            for(playerData pd :net_->getPlayerDataAll()){
                if(pd.type!=net_->getConId() && playerSprite.getGlobalBounds().contains(pd.x,pd.y)){
                    playerSprite.move(-xChange, -yChange);
                    xChange *= -1.f;
                    yChange *= -1.f;
                    playerSprite.move(xChange, yChange);
                }

            }
            
            
            view.move(xChange, yChange);
            window.setView(view);
            text.move(xChange, yChange);
            UItext.move(xChange,yChange);



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

            if(net_->isConnected()){
                net_->refreshData(playerData{(int)playerX,(int)playerY,(int)rot,net_->getConId()});
                net_->refreshAssetData(projectiles_);

                for(std::pair<int,int> pro :net_->getProjectileDataAll()){
                    //std::cout<<std::to_string(pd.x)<< ";"<<std::to_string(pd.y)<<";"<<std::to_string(pd.r)<<std::endl;

                    netprojSprite.setPosition(pro.first,pro.second);
                    window.draw(netprojSprite);
                }


                for(playerData pd :net_->getPlayerDataAll()){
                    //std::cout<<std::to_string(pd.x)<< ";"<<std::to_string(pd.y)<<";"<<std::to_string(pd.r)<<std::endl;
                    nameTag.setString(pd.name);
                    nameTag.setPosition(pd.x-10,pd.y-40);
                    netSprite.setPosition(pd.x,pd.y);
                    netSprite.setRotation(pd.r);
                    if(pd.type!=net_->getConId()){
                        window.draw(netSprite);
                        window.draw(nameTag);
                    }
                }

            }



            for(auto it : projectiles_){
                window.draw(it.sprite_);
            }
            window.draw(playerSprite);
            window.draw(text);
            window.draw(UItext);
            window.display();
        }
        return 0;
    };

private:
    Map map_;
    Player player_;
    Network* net_;
    std::vector<Projectile> projectiles_;
    sf::Clock shootingClock;
    std::vector<Projectile> netProjectiles_;
};

std::vector<std::string> mainMenu(sf::RenderWindow& window, int windowX = 800, int windowY = 600){
    int menuOpt = -1;
    int tmpOpt = 0;
    int valueToEdit = 0;
    int inputTextVal= 0;
    std::vector<std::string> resvector;
    resvector.push_back ("127.0.0.1");
    resvector.push_back ("unknown peli ukko");
    std::vector<std::string> inputTextvector;
    inputTextvector.push_back ("Address");
    inputTextvector.push_back ("Player Name");
    std::vector<std::string> myvector;
    myvector.push_back ("Player Name");
    myvector.push_back ("New Game");
    myvector.push_back ("Network Play");
    myvector.push_back ("Quit");


    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        return resvector;
    }
    sf::Texture menuTEX;
    if (!menuTEX.loadFromFile("assets/menu.png"))
    {
        return resvector;
    }
    sf::Sprite menuS;
    menuS.setTexture(menuTEX);
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    //text.setPosition(10, 20);
    std::string textInput;
    bool textInputOn = false;
    window.setFramerateLimit(12);

    while (menuOpt!=3 && window.isOpen()){
        bool allowReturn = true;
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
        window.clear();
        window.draw(menuS);


        if(textInputOn){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                textInputOn = false;
                menuOpt = -1;
                allowReturn = false;
            } else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                    resvector[valueToEdit] = resvector[valueToEdit].substr(0, resvector[valueToEdit].size()-1);
                } else if (event.type == sf::Event::TextEntered) {
                    resvector[valueToEdit] +=event.text.unicode;
                }
            
                //window.clear();
                text.setPosition(10, 20);
                text.setString(inputTextvector[inputTextVal]+":\n");
                window.draw(text);
                text.setPosition(10, 54);
                text.setString(resvector[valueToEdit]);
                window.draw(text);
                window.display();
                continue;  
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            tmpOpt-=1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            tmpOpt+=1;


        if (tmpOpt<0) tmpOpt=0;
        else if (tmpOpt>=myvector.size())tmpOpt=myvector.size()-1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && allowReturn)
            menuOpt = tmpOpt;


    // set conn addr
        if(menuOpt == 0){
            textInputOn = true;
            valueToEdit = 1;
            inputTextVal = 1;
        } else if(menuOpt == 2){
            textInputOn = true;
            valueToEdit = 0;
            inputTextVal = 0;
            //menuOpt = 3;
        } else if(menuOpt == 1){
            //resvector[valueToEdit] = "127.0.0.1";
            menuOpt = 3;
        } else if(menuOpt == 3){
            window.close();
        } 


        //window.clear();
        for(int menu1 = 0; menu1<myvector.size(); menu1++){
            text.setFillColor(sf::Color::White);
            if (menu1 == tmpOpt){
                text.setFillColor(sf::Color::Red);
            }
            text.setPosition(10, 20+menu1*24);
            text.setString(myvector[menu1]);
            
            window.draw(text);

        }
        
        window.display();
    }
    




}


int main()
{
    int windowX = 800;
    int windowY = 600;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "WIDEO PELI");

    // jokin dynaaminen menu viritelmä tähän väliin?

    std::vector<std::string> menuOpts = mainMenu(window,windowX, windowY);




    //näiden arvojen asettamiset & funktioiden ajamiset voisi tapahtua menun kautta
    std::string addr=menuOpts[0];
    std::string name=menuOpts[1];
    std::cout << addr << "  " << name << std::endl;
    Map kartta("map2.map");

    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);

    Player pelaaja(name, "", ajoneuvo);



    Network aa(addr);
    if(addr!="n") aa.connect(pelaaja);
    Network *verkko = &aa;


    Game peli(kartta, pelaaja, verkko);

    int res = peli.run(window,windowX, windowY);

    return res;
}
