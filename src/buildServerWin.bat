g++ -I"SFML-2.5.1\include"  -c server.cpp
g++ -L"SFML-2.5.1\lib" server.o -o serv -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
