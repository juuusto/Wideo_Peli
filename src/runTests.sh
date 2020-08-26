g++-7 -c main-test.cpp
g++-7 main-test.o -o tests -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
./tests