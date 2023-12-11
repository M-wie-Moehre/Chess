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

string ipAdressString;
Text ipAdressText;

string ipAdressInputString = "";
Text ipAdressInputText;

Text creatingGameText;

// choose online mode functions

// logic functions
void updateChooseOnlineMode()
{
    ipAdressString = IpAddress::getLocalAddress().toString();
    ipAdressText.setString("Your IP address [click to copy]: " + ipAdressString);
}

bool isIpAddressValid()
{
    int tempIpAddress = 0;
    int numberCount = 0;

    for (int i = 0; i < ipAdressInputString.size(); i++)
    {
        if (isdigit(ipAdressInputString[i]))
        {
            tempIpAddress = tempIpAddress * 10;

            tempIpAddress += ipAdressInputString[i] - '0';
        }
        else if (ipAdressInputString[i] == '.')
        {
            if (tempIpAddress < 0 || tempIpAddress > 255)
            {
                return false;
            }
            tempIpAddress = 0;
            numberCount += 1;
        }
        else
        {
            return false;
        }

        if (numberCount > 3)
        {
            return false;
        }
    }

    if (tempIpAddress <= 0 || tempIpAddress >= 255)
    {
        return false;
    }

    if (numberCount != 3)
    {
        return false;
    }

    return true;
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
    ipAdressText.setPosition(Vector2f(windowSizeX * 0.5 - 320, windowSizeY * 0.1));
    ipAdressText.setFillColor(Color(140, 140, 140));
    ipAdressText.setFont(textFont);

    window.draw(ipAdressText);


    if (!ipAdressInputString.empty())
    {
        ipAdressInputText.setString(ipAdressInputString);
        ipAdressInputText.setPosition(Vector2f(windowSizeX * 0.5 - 80, windowSizeY * 0.4));
    }
    else
    {
        ipAdressInputText.setString("Enter IP address to connect to [click to paste].");
        ipAdressInputText.setPosition(Vector2f(windowSizeX * 0.5 - 340, windowSizeY * 0.4));
    }

    ipAdressInputText.setFillColor(Color(140, 140, 140));
    ipAdressInputText.setFont(textFont);

    window.draw(ipAdressInputText);

    // draw the text
    if (creatingGame)
    {
        creatingGameText.setFont(textFont);
        creatingGameText.setString("Wating for someone to join ...");

        creatingGameText.setFillColor(Color(140, 140, 140));
        creatingGameText.setPosition(Vector2f(windowSizeX / 2 - 200, windowSizeY * 0.9));

        window.draw(creatingGameText);
    }
}

#endif