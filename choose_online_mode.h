#ifndef CHOOSE_ONLINE_MODE_H
#define CHOOSE_ONLINE_MODE_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// choose online mode variables

Texture createGameTexture;
Sprite createGameSprite;

Texture joinGameTexture;
Sprite joinGameSprite;

//Text connectionStatusText;

// choose online mode functions

// function to load all texures required for the choose online mode menu
void loadChooseOnlineModeTextures()
{
    if (!createGameTexture.loadFromFile("textures\\create_game.png"))
    {
        cout << "Couldn't load texture \"textures\\create_game.png\". Exiting.." << endl;
        return;
    }

    createGameSprite.setTexture(createGameTexture);
    createGameSprite.setScale(pixelScale * 0.75, pixelScale * 0.75);
    createGameSprite.setPosition(windowSizeX / 2 - (128 * pixelScale * 0.75) / 2, windowSizeY * 0.25 - 32 * pixelScale * 0.75 - 10);

    if (!joinGameTexture.loadFromFile("textures\\join_game.png"))
    {
        cout << "Couldn't load texture \"textures\\join_game.png\". Exiting.." << endl;
        return;
    }

    joinGameSprite.setTexture(joinGameTexture);
    joinGameSprite.setScale(pixelScale * 0.75, pixelScale * 0.75);
    joinGameSprite.setPosition(windowSizeX / 2 - (128 * pixelScale * 0.75) / 2, windowSizeY * 0.25 + 10);
}

// drawing functions

// function to draw the choose online mode menu
void drawChooseOnlineMode(RenderWindow &window)
{
    window.draw(createGameSprite);
    window.draw(joinGameSprite);

    /*// draw the text
    if (creatingGame || joiningGame)
    {
        connectionStatusText.setFont(textFont);
        if (creatingGame)
        {
            connectionStatusText.setString("  Waiting for someone to join ...  ");
        }
        else if (joiningGame)
        {
            connectionStatusText.setString("Trying to join an existing game ...");
        }
        connectionStatusText.setFillColor(Color(140, 140, 140));
        connectionStatusText.setPosition(Vector2f(windowSizeX * 0.5 - 220, windowSizeY * 0.75));

        window.draw(connectionStatusText);
    }*/
}

#endif