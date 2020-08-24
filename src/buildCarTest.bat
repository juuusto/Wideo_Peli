g++  -I"SFML-2.5.1\include" -c cartest.cpp
g++  -L"SFML-2.5.1\lib" cartest.o -o cartest.exe -lsfml-graphics -lsfml-window -lsfml-system 