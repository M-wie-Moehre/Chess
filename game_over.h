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

Texture playAgainTexture;
Sprite playAgainSprite;

Text pressToContinueText;

// game over functions

// function to load all textures required for the game_over screen
void loadGameOverTextures()
{
    // load the text textures
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

    if (!playAgainTexture.loadFromFile("textures\\play_again.png"))
    {
        cout << "Couldn't load texture \"textures\\play_again.png\". Exiting.." << endl;
        return;
    }

    playAgainSprite.setTexture(playAgainTexture);
    playAgainSprite.setScale(pixelScale * 0.75, pixelScale * 0.75); // one pixel on the images is equal to five pixel on the window
    playAgainSprite.setPosition(windowSizeX / 2 - (112 * pixelScale * 0.75) / 2, windowSizeY * 0.75 - 32 * pixelScale * 0.75 - 10);
}

// drawing functions

// function to draw the game_over screen
void drawGameOver(RenderWindow &window)
{
    // draw the game_over message for the respective gameState (1 = white won; 2 = black won; 3 = draw)
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

    window.draw(playAgainSprite);
    window.draw(backSprite);

    // draw the text
    if (playOnline)
    {
        pressToContinueText.setFont(textFont);
        if (!hostReady && !clientReady)
        {
            pressToContinueText.setString("0 / 2 - ready");
        }
        else if (hostReady != clientReady)
        {
            pressToContinueText.setString("1 / 2 - ready");
        }
        else
        {
            pressToContinueText.setString("2 / 2 - ready");
        }

        pressToContinueText.setFillColor(Color(140, 140, 140));
        pressToContinueText.setPosition(Vector2f(windowSizeX / 2 - 80, windowSizeY * 0.9));

        window.draw(pressToContinueText);
    }
}

#endif