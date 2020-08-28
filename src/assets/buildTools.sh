g++  -c mapMaker.cpp
g++  mapMaker.o -o mapEdit
g++  -c srvMapMaker.cpp
g++  srvMapMaker.o -o srvMapEdit

g++  -c collisionMaker.cpp
g++  collisionMaker.o -o tileEdit -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network