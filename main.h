#ifndef MAIN_H
#define MAIN_H

// include SFML library headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// main variables

Color backgroundColor(25, 51, 45);
int pixelScale = 4; // the factor, on how much the textures get scaled up
int windowSizeX = 800;
int windowSizeY = 800;

// current mode (0 = menu; 1 = game; 2 = game over; 3 = choose_online_mode)
#define MENU 0
#define GAME 1
#define GAME_OVER 2
#define CHOOSE_ONLINE_MODE 3

int mode = MENU;


// variables for network usage
bool playOnline = false; // saves if the game is played online or local
bool youAreHost; // saves if you are the host (the one who created the game) or the client (the one who joined the game)
bool hostReady = false;
bool clientReady = false;

TcpSocket socket; // the socket, for the communication between the computers

TcpListener listener;
bool creatingGame = false;

Font textFont;

Texture backTexture;

Sprite backSprite;


// main functions

void loadFonts()
{
    if (!textFont.loadFromFile("fonts\\font.ttf"))
	{
		cout << "Couldn't load text font \"fonts\\font.ttf\". Exiting.." << endl;
		return;
	}
}

void loadMainTextures()
{
	if (!backTexture.loadFromFile("textures\\back.png"))
    {
        cout << "Couldn't load texture \"textures\\back.png\". Exiting.." << endl;
        return;
    }

    backSprite.setTexture(backTexture);
    backSprite.setScale(pixelScale * 0.75, pixelScale * 0.75); // one pixel on the images is equal to five pixel on the window
    backSprite.setPosition(windowSizeX / 2 - (112 * pixelScale * 0.75) / 2, windowSizeY * 0.75 + 10);
}

#endif