#ifndef GAME_OVER_H
#define GAME_OVER_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// game over variables

Texture whiteWinsTextTexture;
Sprite whiteWinsTextSprite;

Texture blackWinsTextTexture;
Sprite blackWinsTextSprite;

Texture drawTextTexture;
Sprite drawTextSprite;

Text pressToContinueText;

// game over functions

// drawing functions
void loadGameOverTextures()
{
    // load the texts
    if (!whiteWinsTextTexture.loadFromFile("textures\\win_white.png"))
    {
        cout << "Couldn't load texture \"textures\\win_white.png\". Exiting.." << endl;
        return;
    }

    whiteWinsTextSprite.setTexture(whiteWinsTextTexture);
    whiteWinsTextSprite.setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
    whiteWinsTextSprite.setPosition(windowSizeX / 2 - (132 * pixelScale) / 2, windowSizeY / 2 - 36 * pixelScale);

    if (!blackWinsTextTexture.loadFromFile("textures\\win_black.png"))
    {
        cout << "Couldn't load texture \"textures\\win_black.png\". Exiting.." << endl;
        return;
    }

    blackWinsTextSprite.setTexture(blackWinsTextTexture);
    blackWinsTextSprite.setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
    blackWinsTextSprite.setPosition(windowSizeX / 2 - (132 * pixelScale) / 2, windowSizeY / 2 - 36 * pixelScale);

    if (!drawTextTexture.loadFromFile("textures\\draw.png"))
    {
        cout << "Couldn't load texture \"textures\\draw.png\". Exiting.." << endl;
        return;
    }

    drawTextSprite.setTexture(drawTextTexture);
    drawTextSprite.setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
    drawTextSprite.setPosition(windowSizeX / 2 - (131 * pixelScale) / 2, windowSizeY / 2 - 36 * pixelScale);
}

void drawGameOver(RenderWindow &window, int gameState)
{
    if (gameState == 1)
    {
        window.draw(whiteWinsTextSprite);
    }
    else if (gameState == 2)
    {
        window.draw(blackWinsTextSprite);
    }
    else if (gameState == 3)
    {
        window.draw(drawTextSprite);
    }

    pressToContinueText.setFont(textFont);
    pressToContinueText.setString("Press to continue ...");
    pressToContinueText.setFillColor(Color(140, 140, 140));
    pressToContinueText.setPosition(Vector2f(windowSizeX / 2 - 120, windowSizeY / 2 + 140));

    window.draw(pressToContinueText);
}

#endif