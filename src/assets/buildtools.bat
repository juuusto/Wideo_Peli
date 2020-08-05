g++  -I"..\SFML-2.5.1\include" -c collisionMaker.cpp
g++  -L"..\SFML-2.5.1\lib" collisionMaker.o -o tileEdit.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

