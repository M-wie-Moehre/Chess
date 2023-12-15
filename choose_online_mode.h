#ifndef CHOOSE_ONLINE_MODE_H
#define CHOOSE_ONLINE_MODE_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// choose online mode variables

// button textures and sprites
Texture createGameTexture;
Sprite createGameSprite;

Texture joinGameTexture;
Sprite joinGameSprite;

// texts for the IP addresses
string ipAdressString;
Text ipAdressText;

string ipAdressInputString = "";
Text ipAdressInputText;

Text creatingGameText;

// choose online mode functions

// logic functions

// function to update the own IP address, since it takes pretty long and is unecessary to do it over and over again
void updateChooseOnlineMode()
{
    ipAdressString = IpAddress::getLocalAddress().toString();
    ipAdressText.setString("Your IP address [click to copy]: " + ipAdressString);
}

// funcntion to check if the entered IP address is valid
bool isIpAddressValid()
{
    int tempIpAddress = 0; // saves the current number of the IP address to check if the number is lower than 256
    int numberCount = 0; // saves how many numbers of the IP address the loop went through

    // go through every character of the IP address
    for (int i = 0; i < ipAdressInputString.size(); i++)
    {
        // if the character is a digit
        if (isdigit(ipAdressInputString[i]))
        {
            // increase the temporary number
            tempIpAddress = tempIpAddress * 10;

            tempIpAddress += ipAdressInputString[i] - '0';
        }
        // if the character is a dot
        else if (ipAdressInputString[i] == '.')
        {
            // check if the temporary number is lower than 256, since it is not allowed
            if (tempIpAddress < 0 || tempIpAddress > 255)
            {
                return false;
            }
            tempIpAddress = 0;
            numberCount += 1;
        }
        // if any other character is entered
        else
        {
            return false;
        }

        // if the IP address is to long
        if (numberCount > 3)
        {
            return false;
        }
    }

    // do the last to steps again, since the IP address doesn't end with a dot
    if (tempIpAddress <= 0 || tempIpAddress >= 255)
    {
        return false;
    }

    if (numberCount != 3)
    {
        return false;
    }

    // if everything was correct, return true
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

    // draw the text with the own IP address
    ipAdressText.setPosition(Vector2f(windowSizeX * 0.5 - 320, windowSizeY * 0.1));
    ipAdressText.setFillColor(Color(140, 140, 140));
    ipAdressText.setFont(textFont);

    window.draw(ipAdressText);


    // draw the text with the other persons IP address
    if (!ipAdressInputString.empty())
    {
        ipAdressInputText.setString(ipAdressInputString);
        ipAdressInputText.setPosition(Vector2f(windowSizeX * 0.5 - 80, windowSizeY * 0.4));
    }
    // if nothing was entered, set a standard text
    else
    {
        ipAdressInputText.setString("Enter IP address to connect to [click to paste].");
        ipAdressInputText.setPosition(Vector2f(windowSizeX * 0.5 - 340, windowSizeY * 0.4));
    }

    ipAdressInputText.setFillColor(Color(140, 140, 140));
    ipAdressInputText.setFont(textFont);

    window.draw(ipAdressInputText);

    // draw the text to indicate, that you are creating a game, if this is the case
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