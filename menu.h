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

Text pressToPlayText;

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
    chessLogoSprite.setPosition(windowSizeX / 2 - (62 * 2 * pixelScale) / 2, windowSizeY / 2 - 16 * 4 * pixelScale); // shift 3 pixels on the image scale further
}

// drawing functions

// function to draw the menu
void drawMenu(RenderWindow &window)
{
    // draw the logo
    window.draw(chessLogoSprite);

    // draw the text
    pressToPlayText.setFont(textFont);
    pressToPlayText.setString("Press to play ...");
    pressToPlayText.setFillColor(Color(140, 140, 140));
    pressToPlayText.setPosition(Vector2f(windowSizeX / 2 - 100, windowSizeY / 2 + 140));

    window.draw(pressToPlayText);
}

#endif