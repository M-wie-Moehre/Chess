#ifndef MENU_H
#define MENU_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// menu variables

Texture chessLogoTexture;
Sprite chessLogoSprite;

Texture playLocalTexture;
Sprite playLocalSprite;
Texture playOnlineTexture;
Sprite playOnlineSprite;

// menu functions

// function to load all texures required for the menu
void loadMenuTextures()
{
    if (!chessLogoTexture.loadFromFile("textures\\logo.png"))
    {
        cout << "Couldn't load texture \"textures\\logo.png\". Exiting.." << endl;
        return;
    }

    chessLogoSprite.setTexture(chessLogoTexture);
    chessLogoSprite.setScale(pixelScale * 2, pixelScale * 2);
    chessLogoSprite.setPosition(windowSizeX / 2 - (62 * 2 * pixelScale) / 2, windowSizeY * 0.25 - (16 * 2 * pixelScale) / 2);


    if (!playLocalTexture.loadFromFile("textures\\play_local.png"))
    {
        cout << "Couldn't load texture \"textures\\play_local.png\". Exiting.." << endl;
        return;
    }

    playLocalSprite.setTexture(playLocalTexture);
    playLocalSprite.setScale(pixelScale * 0.75, pixelScale * 0.75);
    playLocalSprite.setPosition(windowSizeX / 2 - (112 * pixelScale * 0.75) / 2, windowSizeY * 0.75 - 32 * pixelScale * 0.75 - 10);

    if (!playOnlineTexture.loadFromFile("textures\\play_online.png"))
    {
        cout << "Couldn't load texture \"textures\\play_online.png\". Exiting.." << endl;
        return;
    }

    playOnlineSprite.setTexture(playOnlineTexture);
    playOnlineSprite.setScale(pixelScale * 0.75, pixelScale * 0.75);
    playOnlineSprite.setPosition(windowSizeX / 2 - (112 * pixelScale * 0.75) / 2, windowSizeY * 0.75 + 10);
}

// drawing functions

// function to draw the menu
void drawMenu(RenderWindow &window)
{
    // draw the logo
    window.draw(chessLogoSprite);

    window.draw(playLocalSprite);
    window.draw(playOnlineSprite);
}

#endif