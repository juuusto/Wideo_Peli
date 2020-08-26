
#include <SFML/Graphics.hpp>
#include <math.h>

#include <iostream>
#include <stdio.h>
#include <vector>

int main()
{
    int brushSize = 1;
    int windowX = 200;
    int windowY = 200;

    char arr[200][25];
    for (int y = 0; y < 200; y++)
    {
        for (int x = 0; x < 25; x++)
        {
            arr[y][x] = 0;
        }
    }
    int cX = 0;
    int cY = 0;
    

    std::string addr = "";
    std::cout << "Tile collision editor, mouse = set area, s = save to disk, d= display to console, l = load from disk" << std::endl;
    std::cout << "Tile number (number NOT FILENAME): ";
    std::cin >> addr;

    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "WIDEO PELI");
    sf::Texture tex;
    if (!tex.loadFromFile("tile" + addr + ".png"))
    {
        return 0;
    }

    sf::Sprite tileS;
    sf::Sprite cols;

    sf::Texture tex1;
    if (!tex1.create(200, 200))
    {

    }

    sf::Uint8 *pixels = new sf::Uint8[200 * 200 * 4]; // (RGBA)

    for (int y = 0; y < 200; y++)
    {
        for (int x = 0; x < 200; x++)
        {
            pixels[4 * 200 * y + 4 * x + 3] = 0x00;
            pixels[4 * 200 * y + 4 * x + 2] = 0x00;
            pixels[4 * 200 * y + 4 * x + 1] = 0x00;
            pixels[4 * 200 * y + 4 * x + 0] = 0xFF;
        }
    }

    tex1.update(pixels);

    //tileS.move(windowX / 2, windowY / 2);
    tileS.setTexture(tex);
    window.setFramerateLimit(120);
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for(int i = 0; i<brushSize; i++){
                for(int j = 0; j<brushSize; j++){
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    sf::Vector2u wsize = window.getSize();
                    int xcor = 200 * ((1.0f * localPosition.x+i) / wsize.x);
                    int ycor = 200 * ((1.0f * localPosition.y+j) / wsize.y);
                    if (xcor >= 0 && xcor < 200 && ycor >= 0 && ycor < 200)
                    {
                        pixels[4 * 200 * ycor + 4 * xcor + 3] = 0xFF;

                        arr[ycor][xcor / 8] |= (1 << (xcor % 8));
                    }
                }
            }

        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for(int i = 0; i<brushSize; i++){
                for(int j = 0; j<brushSize; j++){
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    sf::Vector2u wsize = window.getSize();
                    int xcor = 200 * ((1.0f * localPosition.x+i) / wsize.x);
                    int ycor = 200 * ((1.0f * localPosition.y+j) / wsize.y);
                    if (xcor >= 0 && xcor < 200 && ycor >= 0 && ycor < 200)
                    {
                        pixels[4 * 200 * ycor + 4 * xcor + 3] = 0x00;

                        arr[ycor][xcor / 8] &= (0xFE << (xcor % 8));
                    }
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {

            for (int y = 0; y < 200; y++)
            {
                for (int x = 0; x < 25; x++)
                {
                    printf("%02x ", arr[y][x] & 0xFF);
                }
                printf("\n");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {

            FILE *file = fopen("tile_tmp.dat", "wb");
            fwrite(arr, 1, 25 * 200, file);
            printf("Saved data to disk\n");
            fclose(file);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            std::string tmp = "tile" + addr + ".dat";
            FILE *file = fopen(tmp.c_str(), "rb");
            fread(&arr, 1, 25 * 200, file);
            printf("Loaded data from disk\n");
            fclose(file);


            for (int y = 0; y < 200; y++)
            {
                for (int x = 0; x < 200; x++)
                {
                    if((arr[y][x/8]&(1<<(x%8))) != 0){
                        pixels[4 * 200 * y + 4 * x + 3] = 0xFF;
                        pixels[4 * 200 * y + 4 * x + 2] = 0x00;
                        pixels[4 * 200 * y + 4 * x + 1] = 0x00;
                        pixels[4 * 200 * y + 4 * x + 0] = 0xFF;

                    }

                }
            }


        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (brushSize<200)brushSize+=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (brushSize>1)brushSize-=1;
        }

        
        window.clear(sf::Color::Black);
        tex1.update(pixels);
        cols.setTexture(tex1);

        window.draw(tileS);
        window.draw(cols);
        window.display();
    }
    delete pixels;
    return 0;
}
