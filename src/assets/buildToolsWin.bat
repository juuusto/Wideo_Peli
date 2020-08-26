g++  -c mapMaker.cpp
g++  mapMaker.o -o mapEdit.exe
g++  -c srvMapMaker.cpp
g++  srvMapMaker.o -o srvMapEdit.exe

g++  -I"..\SFML-2.5.1\include" -c collisionMaker.cpp
g++  -L"..\SFML-2.5.1\lib" collisionMaker.o -o tileEdit.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

