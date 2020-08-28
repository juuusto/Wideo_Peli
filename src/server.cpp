#include <SFML/Network.hpp>
#include <iostream>
#include <ctime>
#include <math.h>
#include <chrono>
#include <fstream>

struct aiplayerServerData
{
    float x;
    float y;
    float r;
    int type;
    float aiSpeed;
    std::time_t connectTimeout;
    std::vector<std::pair<int, int>> path;
    int aiTarget;
    std::string name;
    int lapCount;
};
struct playerServerData
{
    int x;
    int y;
    int r;
    int type;
    std::time_t connectTimeout;
    std::string name;
};
struct projectileData
{
    int x;
    int y;
};


// calculate direction for ai
float calcAIrotDiff(int ox, int oy, int nx, int ny)
{

    float xdiff = nx - ox;
    float ydiff = ny - oy;

    if (xdiff == 0)
    {
        if (ydiff < 0)
            return 90.f;
        else
            return 270.f;
    }
    else if (xdiff < 0)
    {
        return 180 + std::atan(ydiff / xdiff) * 180 / 3.14f;
    }
    else
    {
        return std::atan(ydiff / xdiff) * 180 / 3.14f;
    }
}


// load mapfile
bool loadServerMap(std::string &SRV_MAP, std::vector<std::vector<std::pair<int, int>>> &aiDat, std::string mapname)
{
    std::ifstream mapf("./assets/"+mapname);

    if (mapf.is_open())
    {
        std::cout << "Start map load\n";
        SRV_MAP = "";
        char tmpchar = 0xFF;
        while (true)
        {
            mapf.read(&tmpchar, 1);
            if (tmpchar == 0)
                break;
            SRV_MAP += tmpchar;
        }
        std::cout << "Server mapname loaded " << SRV_MAP << "\n";
        bool readMore = true;

        while (readMore)
        {
            std::vector<std::pair<int, int>> tmpData{};
            while (true)
            {
                int x = 0;
                int y = 0;
                mapf.read((char *)&x, 4);
                mapf.read((char *)&y, 4);
                std::cout << "X:" << x << "Y:" << y << "\n";

                if (x == 0 && y == 0)   // this ai is done, load the next one
                    break;
                else if (x == 0xFFFFFFFF && y == 0xFFFFFFFF) // terminate reading of ai players
                {
                    readMore = false;
                    break;
                }
                tmpData.push_back(std::pair<int, int>(x, y));
            }
            std::cout << " Ai player loaded\n";
            aiDat.push_back(tmpData);
        }
        std::cout << "All Ai players loaded\n";
        mapf.close();
        return true;
    } else {
        return false;
    }
}


// main server function
int main(int argc, char *argv[])
{
    // how many laps to play for
    int winLapCount = 2;
 
    if (argc<3){
        std::cout << "Please give map file name\n Usage:"<<argv[0]<< " <map filename> <pcount>\n";
        return 0;
    }
    // start initializing server variables
    int wantedPcount = std::stoi(argv[2]);
    std::cout <<"Pcount:"<< std::to_string(wantedPcount)<< "\n";
    std::string srv_headerfile_name = argv[1];
    std::vector<std::vector<std::pair<int, int>>> aiData;

    std::string SERVER_MAP;

    if(!loadServerMap(SERVER_MAP, aiData,srv_headerfile_name)){
        std::cout << "Could not start server! Map not found";
        return 0;
    }
    std::cout << "Server map loaded\n";

    unsigned short inPort = 54010;
    unsigned short outPort = 54000;
    sf::UdpSocket socket;
    socket.bind(inPort) != sf::Socket::Done;
    socket.setBlocking(false);

    std::time_t currentTime = std::time(nullptr);
    std::chrono::steady_clock::time_point betterTime = std::chrono::steady_clock::now();
    std::vector<playerServerData> players;
    std::vector<aiplayerServerData> aiPlayers;
    std::vector<std::vector<projectileData>> projData;

    // load ai data to ai structs
    // for AI
    for (int aiPath = 0; aiPath < aiData.size(); aiPath++)
    {
        float aiX = aiData[aiPath][0].first;
        float aiY = aiData[aiPath][0].second;
        aiPlayers.push_back(aiplayerServerData{aiX, aiY, 0, aiPath, 15.f, currentTime, aiData[aiPath], 0, "ai",0});
    }
    std::chrono::steady_clock::time_point aiUptdTime = betterTime;

    // game reset loop, this resets the game when a round ends
    while(true){
        bool gameon = true;
        players.clear();
        for(int aaa = 0; aaa<aiPlayers.size();aaa++){
            aiPlayers[aaa].lapCount=0;
            aiPlayers[aaa].aiTarget=0;
            aiPlayers[aaa].x=aiData[aaa][0].first;
            aiPlayers[aaa].y=aiData[aaa][0].second;
        }
        projData.clear();

        std::cout << "GAME STARTED!\n";
        time_t gameoverTimeout = 0;//std::time(nullptr);
        bool startGame = false;

        // main loop for command and request handling 
        while (gameon || (std::time(nullptr)-gameoverTimeout)<2)
        {
            // don't start before there are enough players connected
            if(players.size() >= wantedPcount)startGame=true;
            currentTime = std::time(nullptr);
            betterTime = std::chrono::steady_clock::now();


            // update ai every 30 ms
            if (std::chrono::duration_cast<std::chrono::milliseconds>(betterTime - aiUptdTime).count() > 30 && startGame)
            {
                aiUptdTime = betterTime;
                for (int ThisAi = 0; ThisAi < aiPlayers.size(); ThisAi++)
                {
                    // if not at next node, move torwards it 
                    if (abs(aiPlayers[ThisAi].x - aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].first) > 20 || abs(aiPlayers[ThisAi].y - aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].second) > 20)
                    {
                        aiPlayers[ThisAi].x += aiPlayers[ThisAi].aiSpeed * cos((aiPlayers[ThisAi].r / 180.f) * 3.14f);
                        aiPlayers[ThisAi].y += aiPlayers[ThisAi].aiSpeed * sin((aiPlayers[ThisAi].r / 180.f) * 3.14f);
                        aiPlayers[ThisAi].connectTimeout = currentTime;
                        //std::cout << "X:"<<aiPlayers[ThisAi].x<<"Y:"<<aiPlayers[ThisAi].y<<"R:"<< aiPlayers[ThisAi].r<<std::endl;
                    }
                    else // at a node, set the next as target
                    {
                        aiPlayers[ThisAi].aiTarget++;
                        aiPlayers[ThisAi].aiTarget = aiPlayers[ThisAi].aiTarget % aiPlayers[ThisAi].path.size();


                        std::cout << "Aitarget:"<<std::to_string(aiPlayers[ThisAi].aiTarget)<< " x:"<<  aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].first << " y:"<< aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].second<<"\n";
                        if(aiPlayers[ThisAi].aiTarget == 1)aiPlayers[ThisAi].lapCount += 1;
                        if(aiPlayers[ThisAi].lapCount == winLapCount){
                            gameon=false;
                            gameoverTimeout = std::time(nullptr);
                        }
                        aiPlayers[ThisAi].r = calcAIrotDiff(aiPlayers[ThisAi].x, aiPlayers[ThisAi].y, aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].first, aiPlayers[ThisAi].path[aiPlayers[ThisAi].aiTarget].second);
                    }
                }
            }
            // receive next command/ request
            sf::IpAddress sender;
            char data[1300];
            std::size_t received;
            unsigned short port;
            if (socket.receive(data, 1300, received, sender, port) != sf::Socket::Done)
            {
                // error...
            }

            std::string s(data);

            if (received == 0)
                continue;
            //std::cout << s<<std::endl;
            std::string delimiter = ";";
            size_t pos = 0;
            int i = 0;
            std::string command;
            std::vector<std::string> tmp_arr;

            // parse request
            while ((pos = s.find(delimiter)) != std::string::npos)
            {
                command = s.substr(0, pos);
                //std::cout << command << std::endl;
                tmp_arr.push_back(command);
                s.erase(0, pos + delimiter.length());
                i += 1;
            }
            
            // handle the given command
            std::string res;
            if (tmp_arr[0] == "CONNECT")
            {
                std::cout << "Connection... \n"
                        << std::to_string(players.size()) << std::endl;
                int pCount = 0;
                for (playerServerData pd : players)
                {
                    if (currentTime - pd.connectTimeout < 2)
                        pCount += 1;
                }

                if (pCount == 0)
                {
                    players.clear();
                }

                if (pCount < wantedPcount)
                {
                    projData.push_back(std::vector<projectileData>());
                    players.push_back(playerServerData{100, 100, 0, (int)players.size(), currentTime, tmp_arr[1]});
                    res = "OK;" + std::to_string(players.size() - 1) + ";" + SERVER_MAP + ";"+std::to_string(winLapCount)+";";
                }
                else
                {
                    res = "ERROR;TOO MANY PLAYERS;";
                }
            }

            if (tmp_arr[0] == "UPDATE")
            {
                //std::cout << "Update... \n"<<std::endl;
                players[stoi(tmp_arr[4])] = {stoi(tmp_arr[1]), stoi(tmp_arr[2]), stoi(tmp_arr[3]), stoi(tmp_arr[4]), currentTime, players[stoi(tmp_arr[4])].name};
                res = "";

                // human players
                for (playerServerData pd : players)
                {
                    if (currentTime - pd.connectTimeout < 2)
                        res += std::to_string(pd.x) + ";" + std::to_string(pd.y) + ";" + std::to_string(pd.r) + ";" + std::to_string(pd.type) + ";" + pd.name + ";";
                }
                // add ai to the response
                for (aiplayerServerData pd : aiPlayers)
                {
                    int ax = pd.x;
                    int ay = pd.y;
                    int ar = pd.r;
                    res += std::to_string(ax) + ";" + std::to_string(ay) + ";" + std::to_string(ar) + ";" + std::to_string(100 + pd.type) + ";" + pd.name + ";";
                }
                std::cout <<res<<"\n";
            }
            else if (tmp_arr[0] == "UASSETS") // update projectiles
            {
                std::cout << "Update projectiles... \n"
                        << std::endl;

                projData[stoi(tmp_arr[1])].clear();
                for (int pri = 2; pri < tmp_arr.size(); pri += 2)
                {
                    projData[stoi(tmp_arr[1])].push_back({stoi(tmp_arr[pri]), stoi(tmp_arr[pri + 1])});
                }

                res = "";
                for (std::vector<projectileData> pr : projData)
                {
                    for (projectileData pr1 : pr)
                    {
                        std::cout << std::to_string(pr1.x) + ";" + std::to_string(pr1.y) + ";";
                        res += std::to_string(pr1.x) + ";" + std::to_string(pr1.y) + ";";
                    }
                }
                std::cout << std::endl;
            }
            else if (tmp_arr[0] == "PLAYERS")
            {
                //std::cout << "Sending Player list... \n"<<std::endl;
                res = "";
                int pCount = 0;
                for (playerServerData pd : players)
                {
                    if (currentTime - pd.connectTimeout < 2)
                        pCount += 1;
                }

                res += "PLAYERCOUNT;" + std::to_string(pCount) + ";";
            }
            // handle ending the game
            else if (tmp_arr[0] == "IWIN")
            {
                gameoverTimeout = std::time(nullptr);
                std::cout << "game ended\n";
                res = "OK;";
                gameon = false;

            }
            // give status of game server
            else if (tmp_arr[0] == "GAMESTATUS")
            {
                players[stoi(tmp_arr[1])].connectTimeout=currentTime;
                if(!startGame)res ="HOLD;";
                else if(gameon)res = "KEEPGOING;";
                else res = "DONEDEAL;";

            }
            // force start game, not in use currently
            else if (tmp_arr[0] == "START")
            {
                startGame = true;
                res = "OK;";

            }
            // send the response
            if (socket.send(res.c_str(), res.size() + 1, sender, outPort) != sf::Socket::Done)
            {
                // error...
            }
            //std::cout << "Time: "<< currentTime <<std::endl;
        }
    }
}
