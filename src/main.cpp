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
    Game(Map map, Player player, Network *net) : map_(map), player_(player), net_(net){

                                                                             };

    ~Game()
    {
    }
    int run(sf::RenderWindow &window,bool debugMode, int windowX = 800, int windowY = 600, int startX = 425, int startY = 312)
    {


        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("assets/arial.ttf"))
        {
            return 0;
        }
        sf::Sprite endS;
        sf::Texture waitTEX;
        if (!waitTEX.loadFromFile("assets/wait.png"))
        {
            return 0;
        }

        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(10, 20);

        while(net_->gameStatus()==2){
            window.clear();
            //text.setString("WAIT");
            //window.draw(text);
            endS.setTexture(waitTEX);
            window.draw(endS);
            window.display();
        }
        int currentLap = 1;
        int currentCheckpoint= 1;
        int checkpointMAX = 3;
        int winLapCount = net_->getLapCount();
        int selectedType = 0;
        std::string gameEnd = "";
        sf::View view(sf::FloatRect(0.f, 20.f, windowX, windowY - 20.f));
        sf::Texture winTEX;
        if (!winTEX.loadFromFile("assets/win.png"))
        {
            return 0;
        }
        sf::Texture loseTEX;
        if (!loseTEX.loadFromFile("assets/lose.png"))
        {
            return 0;
        }
        sf::Texture dedTEX;
        if (!loseTEX.loadFromFile("assets/ded.png"))
        {
            return 0;
        }

        
        std::vector<sf::Texture> textures;
        for (auto tx : map_.getTiles())
        {
            sf::Texture aaa;
            
            aaa.loadFromFile(tx.getTileName());
            textures.push_back(aaa);
        }
        sf::Texture boostText;
        boostText.loadFromFile("assets/boost.png");
        sf::Sprite boostsprite_;
        boostsprite_.setTexture(boostText);

        sf::Sprite tarsprite_;
        sf::Texture tarText;
        tarText.loadFromFile("assets/tar.png");
        tarsprite_.setTexture(tarText);

        sf::Text UItext;

        UItext.setFont(font);
        UItext.setCharacterSize(24);
        UItext.setFillColor(sf::Color::Red);
        UItext.setPosition(10, 560);

        sf::Text nameTag;
        nameTag.setFont(font);
        nameTag.setCharacterSize(16);
        nameTag.setFillColor(sf::Color::White);

        window.setView(view);

        float currentSpeed = 0.f;
        float currentSpeedSide = 0.f;

        // loading textures 
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

        for (int i = 0; i < map_.getMapWidth(); i++)
        {
            for (int j = 0; j < map_.getMapHeight(); j++)
            {
                int rand0 = rand() % 15;
                int rand1 = rand() % 160 + 25;
                int rand2 = rand() % 160 + 25;
                if (rand0 < 3  && !map_.getTile(i, j).isCollision(rand1,rand2))
                {
                    tarSpills_.push_back(std::pair<int, int>(i * map_.getBlockSize() + rand1, j * map_.getBlockSize() + rand2));
                }
            }
        }

        for (int i = 0; i < map_.getMapWidth(); i++)
        {
            for (int j = 0; j < map_.getMapHeight(); j++)
            {
                int rand3 = rand() % 15;
                    int rand4 = rand() % 160 + 25;
                    int rand5 = rand() % 160 + 25;
                if (rand3 < 3 && !map_.getTile(i, j).isCollision(rand4,rand5))
                {
                    boosts_.push_back(std::pair<int, int>(i * map_.getBlockSize() + rand4, j * map_.getBlockSize() + rand5));
                }
            }
        }

        sf::Texture projTexture;
        if (!projTexture.loadFromFile("assets/projectile.png"))
        {
            return 0;
        }

        sf::SoundBuffer shootingBuff;
        if (!shootingBuff.loadFromFile("assets/shoot.wav"))
        {
            return 0;
        }

        sf::Music driveSound;
        if (!driveSound.openFromFile("assets/drive.wav"))
        {
            return 0;
        }

        driveSound.setVolume(10.f);
        driveSound.setLoop(true);
        sf::Sound shootingSound;
        shootingSound.setBuffer(shootingBuff);
        shootingSound.setVolume(10.f);

        sf::Music mapMusic;
        if (!mapMusic.openFromFile("assets/" + map_.getMapMusicFile()))
        {
            return 0;
        }
        mapMusic.setVolume(10.f);
        mapMusic.setLoop(true);
        mapMusic.play();

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

        // move player to starting location

        int startX1 = startX - 425;
        int startY1 = startY - 312;
        playerSprite.move(startX1, startY1);
        view.move(startX1, startY1);
        window.setView(view);
        text.move(startX1, startY1);
        UItext.move(startX1, startY1);

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
            {
                if(driveSound.getStatus() != driveSound.Playing) driveSound.play();
                currentSpeed += player_.getCar().getAcceleration();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                currentSpeed -= player_.getCar().getAcceleration();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                rot -= player_.getCar().getTurnSpeed();
                currentSpeedSide += 0.1f*currentSpeed;
                currentSpeed *=0.93f;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                rot += player_.getCar().getTurnSpeed();
                currentSpeedSide -= 0.1f*currentSpeed;
                currentSpeed *=0.93f;
            }
            if(player_.getHp()<1)gameEnd="YOU DED";

            if(gameEnd != ""){
                view.setCenter(400,300);
                window.setView(view);
                window.clear();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)){
                    if(net_->connect(player_))return 1;
                    else return 0;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
                    return 0;
                }
                    
                if(gameEnd == "YOU ALMOST WON" || gameEnd == "YOU LOST"){
                    endS.setTexture(loseTEX);
                    window.draw(endS);
                } else if(gameEnd == "YOU WIN"){
                    endS.setTexture(winTEX);
                    window.draw(endS);
                }else if(gameEnd == "YOU DED"){
                    endS.setTexture(dedTEX);
                    window.draw(endS);
                }else {
                    text.setString(gameEnd);
                    window.draw(text);
                }
                text.setString("New game y/n ?");
                text.setPosition(50, 50);
                window.draw(text);
                window.display();
                continue;
            }

            //shooting variables
            Projectile projectile;
            projectile.sprite_.setTexture(projTexture);
            sf::Vector2f aimDirection;
            sf::Vector2f aimDirectionNorm;
            sf::Vector2f playerSpriteCenter;

            playerSpriteCenter = sf::Vector2f(playerSprite.getPosition().x, playerSprite.getPosition().y);
            aimDirection = sf::Vector2f(cos(playerSprite.getRotation() * (3.14 / 180)), sin(playerSprite.getRotation() * (3.14 / 180)));
            aimDirectionNorm = aimDirection / sqrt((aimDirection.x * aimDirection.x) + (aimDirection.y * aimDirection.y));

            //shooting

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player_.getAmmo() > 0 && shootingClock.getElapsedTime().asSeconds() > 2)
            {
                shootingClock.restart();
                player_.shoot();
                shootingSound.play();
                projectile.sprite_.setPosition(playerSpriteCenter);
                projectile.speed_ = aimDirectionNorm * (projectile.maxSpeed_+currentSpeed);
                projectiles_.push_back(Projectile(projectile));
            }

            //erasing when projectiles are out of map
            for (size_t i = 0; i < projectiles_.size(); i++)
            {
                projectiles_[i].sprite_.move(projectiles_[i].speed_);

                if (projectiles_[i].sprite_.getPosition().x < 0 || projectiles_[i].sprite_.getPosition().x > map_.getMapWidth() * map_.getBlockSize() ||
                    projectiles_[i].sprite_.getPosition().y < 0 || projectiles_[i].sprite_.getPosition().y > map_.getMapHeight() * map_.getBlockSize())
                {
                    projectiles_.erase(projectiles_.begin() + i);
                }
            }

            //checking for other players projectiles hitting you
           /* for (size_t i = 0; i < netProjectiles_.size(); i++)
            {
                if (netProjectiles_[i].sprite_.getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
                {
                    player_.hit();
                    netProjectiles_.erase(netProjectiles_.begin() + i);
                }
            }*/

            currentSpeed -= player_.getCar().getDrag();
            currentSpeedSide *= 0.85f;

            if (currentSpeed < 0.f)
            {
                driveSound.pause();
                currentSpeed = 0.f;
            }

            else if (currentSpeed > player_.getCar().getMaxSpeed() && boostClock.getElapsedTime().asMilliseconds() > 700)
            {
                currentSpeed = player_.getCar().getMaxSpeed();
            }

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

            int offsetInTileX = playerX - blockX * map_.getBlockSize();
            int offsetInTileY = playerY - blockY * map_.getBlockSize();

            float xChange = currentSpeed * cos((rot / 180.f) * 3.14f);
            float yChange = currentSpeed * sin((rot / 180.f) * 3.14f);
            xChange += currentSpeedSide * cos(((rot+90) / 180.f) * 3.14f);
            yChange += currentSpeedSide * sin(((rot+90) / 180.f) * 3.14f);


            if (playerX < 0 || playerY < 0 || blockX >= map_.getMapWidth() || blockY >= map_.getMapHeight())
            {
                xChange *= -3.f;
                yChange *= -3.f;
            }
            else if (map_.getTile(blockX, blockY).isCollision(offsetInTileX, offsetInTileY))
            {
                // if collision, dont allow move (reverse move)
                xChange *= -3.f;
                yChange *= -3.f;
                int blockXT = (playerX+xChange) / map_.getBlockSize();
                int blockYT = (playerY+yChange) / map_.getBlockSize();

                int offsetInTileXT = playerX+xChange - blockXT * map_.getBlockSize();
                int offsetInTileYT = playerY+yChange - blockYT * map_.getBlockSize();

                if(map_.getTile(blockXT, blockYT).isCollision(offsetInTileXT, offsetInTileYT)){
                    xChange = 0;
                    yChange = 0;
                }

            }

// checkpoints
int checkHere = map_.getTileCheckpoint( blockX, blockY);
if (checkHere == currentCheckpoint+1){
    currentCheckpoint++;
} else if (checkHere == 1 && currentCheckpoint == checkpointMAX){
    currentLap++;
    currentCheckpoint = 1;
}


if(currentLap == winLapCount){
    if(net_->iWin()){
        gameEnd = "YOU WIN";
    } else {
        gameEnd = "YOU ALMOST WON";
    }
    //currentLap = 20;
} else {
    if(net_->gameStatus() == 0) {
        gameEnd = "YOU LOST";
    }
}

            if(debugMode){
                text.setString(
                "PLAYING AS: " + player_.getName() + " BLOCK X:" + std::to_string(blockX) + " Y:" + std::to_string(blockY) + " Type:" + std::to_string(map_.getTileId(blockX, blockY)) + "   " + std::to_string(boostClock.getElapsedTime().asSeconds()) +
                "\nW:" + std::to_string(view.getSize().x) + " H:" + std::to_string(view.getSize().y) + " OffsetXinTile:" + std::to_string(offsetInTileX) + " OffsetYinTile:" + std::to_string(offsetInTileY)+
                "\n playerX:"+std::to_string(playerSprite.getPosition().x)+" playerY:"+std::to_string(playerSprite.getPosition().y) +"\nspeedForward:"+std::to_string(currentSpeed)+"speedSide"+std::to_string(currentSpeedSide)+
                "\nLap:"+std::to_string(currentLap) +"/"+std::to_string(net_->getLapCount())+ " cPoint:"+std::to_string(currentCheckpoint)+ " tilecPoint:"+std::to_string(checkHere));

            } else {
                text.setString("");
            }

            UItext.setString(
                "Ammo:" + std::to_string(player_.getAmmo()) + " Weapon cooldown: " + std::to_string(shootingClock.getElapsedTime().asSeconds()) + " HP: " + std::to_string(player_.getHp())+
                " Lap:"+std::to_string(currentLap) +"/"+std::to_string(net_->getLapCount()));

            playerSprite.move(xChange, yChange);
            for (playerData pd : net_->getPlayerDataAll())
            {
                if (pd.type != net_->getConId() && playerSprite.getGlobalBounds().contains(pd.x, pd.y))
                {
                    playerSprite.move(-xChange, -yChange);
                    xChange *= -1.f;
                    yChange *= -1.f;
                    playerSprite.move(xChange, yChange);
                }
            }

            view.move(xChange, yChange);
            window.setView(view);
            text.move(xChange, yChange);
            UItext.move(xChange, yChange);

            window.clear();

            int drawDistance = 3;
            for (int i = blockX - drawDistance; i < blockX + drawDistance; i++)
            {
                for (int j = blockY - drawDistance; j < blockY + drawDistance; j++)
                {
                    sf::Sprite drawTile;
                    if (i < 0 || j < 0 || i >= map_.getMapWidth() || j >= map_.getMapHeight())
                        continue;

                    drawTile.setTexture(textures[map_.getTileId(i, j)]);
                    drawTile.move(i * map_.getBlockSize(), j * map_.getBlockSize());
                    window.draw(drawTile);
                }
            }

            if (net_->isConnected())
            {
                net_->refreshData(playerData{(int)playerX, (int)playerY, (int)rot, net_->getConId()});
                net_->refreshAssetData(projectiles_);

                for (std::pair<int, int> pro : net_->getProjectileDataAll())
                {
                    //std::cout<<std::to_string(pd.x)<< ";"<<std::to_string(pd.y)<<";"<<std::to_string(pd.r)<<std::endl;

                    netprojSprite.setPosition(pro.first, pro.second);
                    if(netprojSprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds()) && shootingClock.getElapsedTime().asMilliseconds() > 200){
                        player_.hit();
                    }

                    window.draw(netprojSprite);

                    //if(netprojSprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds())){
                    //    player_.hit();
                    //    netProjectiles_.erase(netProjectiles_.begin() + i);
                    //}
                }

                for (playerData pd : net_->getPlayerDataAll())
                {
                    //std::cout<<std::to_string(pd.x)<< ";"<<std::to_string(pd.y)<<";"<<std::to_string(pd.r)<<std::endl;
                    nameTag.setString(pd.name);
                    nameTag.setPosition(pd.x - 10, pd.y - 40);
                    netSprite.setPosition(pd.x, pd.y);
                    netSprite.setRotation(pd.r);
                    if (pd.type != net_->getConId())
                    {
                        window.draw(netSprite);

                        for (size_t i = 0; i < projectiles_.size(); i++){
                            if(projectiles_[i].sprite_.getGlobalBounds().intersects(netSprite.getGlobalBounds())){
                                projectiles_.erase(projectiles_.begin() + i);
                            }
                        }

                        window.draw(nameTag);
                    }
                }
            }

            for (auto it : tarSpills_)
            {
                tarsprite_.setPosition(it.first, it.second);
                window.draw(tarsprite_);
                if (tarsprite_.getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
                {
                    currentSpeed= 0;

                }
            }

            for (auto it : boosts_)
            {
                boostsprite_.setPosition(it.first, it.second);
                window.draw(boostsprite_);
                if (boostsprite_.getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
                {
                    boostClock.restart();
                    currentSpeed = 14 ;
                }
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
    Network *net_;
    std::vector<std::pair<int, int>> tarSpills_;
    std::vector<std::pair<int, int>> boosts_;
    sf::Clock boostClock;
    std::vector<Projectile> projectiles_;
    sf::Clock shootingClock;
    std::vector<Projectile> netProjectiles_;
};

std::vector<std::string> mainMenu(sf::RenderWindow &window,std::string menuText, std::string addr,std::string name,int windowX = 800, int windowY = 600)
{
    int menuOpt = -1;
    int tmpOpt = 0;
    int valueToEdit = 0;
    int inputTextVal = 0;
    std::vector<std::string> resvector;
    resvector.push_back(addr);
    resvector.push_back(name);
    resvector.push_back("");
    std::vector<std::string> inputTextvector;
    inputTextvector.push_back("Address");
    inputTextvector.push_back("Player Name");
    std::vector<std::string> myvector;
    myvector.push_back("Player Name");
    myvector.push_back("Connect");
    myvector.push_back("Set Address");
    myvector.push_back("Quit");

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

    while (menuOpt != 3 && window.isOpen())
    {
        bool allowReturn = true;
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                resvector[2] = "GAME_EXIT";
                menuOpt = 3;
                break;
            default:
                break;
            }
        }
        window.clear();
        window.draw(menuS);

        if (textInputOn)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                textInputOn = false;
                menuOpt = -1;
                allowReturn = false;
            }
            else
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    resvector[valueToEdit] = resvector[valueToEdit].substr(0, resvector[valueToEdit].size() - 1);
                }
                else if (event.type == sf::Event::TextEntered)
                {
                    resvector[valueToEdit] += event.text.unicode;
                }

                //window.clear();
                text.setPosition(10, 20);
                text.setString(inputTextvector[inputTextVal] + ":\n");
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
            tmpOpt -= 1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            tmpOpt += 1;

        if (tmpOpt < 0)
            tmpOpt = 0;
        else if (tmpOpt >= myvector.size())
            tmpOpt = myvector.size() - 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && allowReturn)
            menuOpt = tmpOpt;

        // set conn addr
        if (menuOpt == 0)
        {
            textInputOn = true;
            valueToEdit = 1;
            inputTextVal = 1;
        }
        else if (menuOpt == 2)
        {
            textInputOn = true;
            valueToEdit = 0;
            inputTextVal = 0;

        }
        else if (menuOpt == 1)
        {

            menuOpt = 3;
            resvector[2] = "GAME_CONNECT";
        }
        else if (menuOpt == 3)
        {
            resvector[2] = "GAME_EXIT";
            window.close();
        }

        //window.clear();
        for (int menu1 = 0; menu1 < myvector.size(); menu1++)
        {
            text.setFillColor(sf::Color::White);
            if (menu1 == tmpOpt)
            {
                text.setFillColor(sf::Color::Red);
            }
            text.setPosition(10, 20 + menu1 * 24);
            text.setString(myvector[menu1]);

            window.draw(text);
        }
        if(menuText!=""){
            text.setFillColor(sf::Color::White);
            text.setPosition(350, 570);
            text.setString(menuText);
            window.draw(text);
        }
        window.display();
    }
}

int main(int argc, char *argv[])
{
    std::string addr ="127.0.0.1";
    std::string name ="unknown peli ukko";
    bool inMenu = true;
    bool debugMode = false;
    if(argc>1){

        for(int argi = 1; argi< argc; argi++){
            std::string com = "";
            //std::cout << argv[argi]<<"\n";
            if(argv[argi][0] =='-'){
                com =argv[argi];
                if(com == "-nomenu"){
                    inMenu = false;
                } else if(com == "-addr"){
                    argi++;
                    if(argi< argc)addr =argv[argi];
                }else if(com == "-name"){
                    argi++;
                    if(argi< argc)name =argv[argi];
                } else if(com == "-debug"){
                    debugMode = true;
                }
            }

            


        }
    }
    int res = -1;
    int windowX = 800;
    int windowY = 600;
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "WIDEO PELI");

    
    std::string menuMessage = "";
    Vehicle ajoneuvo("assets/car.png", 10.f, 1.f, 5.f, .5f);
    Player pelaaja("tmp", "", ajoneuvo);
    Network verkko;
    if(!inMenu){
        pelaaja.setName(name);
        verkko.setAddress(addr);
        inMenu =!verkko.connect(pelaaja);
        menuMessage ="Could not connect!";
    }
    while(inMenu){

        
        std::vector<std::string> menuOpts = mainMenu(window,menuMessage, addr,name, windowX, windowY);
        

        addr = menuOpts[0];
        name = menuOpts[1];
        std::cout << addr << "  " << name << std::endl;

        pelaaja.setName(name);

        verkko.setAddress(addr);
        if (menuOpts[2] == "GAME_EXIT"){
            inMenu=false; 
            return 0;
            //break;
        }
        else if (menuOpts[2] == "GAME_CONNECT" && verkko.connect(pelaaja)) {
            inMenu=false; 
            
           // break;
        }
        else menuMessage ="Could not connect!";
    }
    
    
    while (res!=0){
        Map kartta(verkko.getServerMap());

        Game peli(kartta, pelaaja, &verkko);

        res = peli.run(window,debugMode, windowX, windowY, kartta.getStartLoc().first, kartta.getStartLoc().second);
    }


    return res;
}
