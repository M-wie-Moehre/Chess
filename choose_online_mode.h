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

Text ipAdressText;

string ipAdressInput = "";
Text ipAdressInputText;

// choose online mode functions

// logic functions
void updateChooseOnlineMode()
{
    string yourIP = "Your IP address: " + IpAddress::getLocalAddress().toString();
    ipAdressText.setString(yourIP);
}

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
    createGameSprite.setPosition(windowSizeX / 2 - (128 * pixelScale * 0.75) / 2, windowSizeY * 0.175);

    if (!joinGameTexture.loadFromFile("textures\\join_game.png"))
    {
        cout << "Couldn't load texture \"textures\\join_game.png\". Exiting.." << endl;
        return;
    }

    joinGameSprite.setTexture(joinGameTexture);
    joinGameSprite.setScale(pixelScale * 0.75, pixelScale * 0.75);
    joinGameSprite.setPosition(windowSizeX / 2 - (128 * pixelScale * 0.75) / 2, windowSizeY * 0.475);
}

// drawing functions

// function to draw the choose online mode menu
void drawChooseOnlineMode(RenderWindow &window)
{
    window.draw(createGameSprite);
    window.draw(joinGameSprite);

    window.draw(backSprite);

    // draw the text
    ipAdressText.setPosition(Vector2f(windowSizeX * 0.5 - 220, windowSizeY * 0.1));
    ipAdressText.setFillColor(Color(140, 140, 140));
    ipAdressText.setFont(textFont);

    window.draw(ipAdressText);


    if (!ipAdressInput.empty())
    {
        ipAdressInputText.setString(ipAdressInput);
    }
    else
    {
        ipAdressInputText.setString("Type to enter IP address to connect to.");
    }

    ipAdressInputText.setPosition(Vector2f(windowSizeX * 0.5 - 280, windowSizeY * 0.4));
    ipAdressInputText.setFillColor(Color(140, 140, 140));
    ipAdressInputText.setFont(textFont);

    window.draw(ipAdressInputText);
}

#endif