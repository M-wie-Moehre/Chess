#ifndef MAIN_H
#define MAIN_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// main variables

Color backgroundColor(25, 51, 45);
int pixelScale = 4; // the factor, on how much the textures get scaled up
int windowSizeX = 800;
int windowSizeY = 800;

// current mode (0 = main menu; 1 = game; 2 = game over)
int mode = 0;

Font textFont;

// main functions

void loadFonts()
{
    if (!textFont.loadFromFile("fonts\\font.ttf"))
	{
		cout << "Couldn't load text font \"fonts\\font.ttf\". Exiting.." << endl;
		return;
	}
}

#endif