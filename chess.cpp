// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// include the custom game headers
#include "main.h"
#include "menu.h"
#include "game.h"
#include "game_over.h"

int main()
{
	// load all assets
	// load all fonts
	loadFonts();

	// load all textures
	loadMenuTextures();

	loadGameTextures();

	loadGameOverTextures();

    // create and open a new graphics window of size 800x800 Pixel and a title
	RenderWindow window(VideoMode(windowSizeX, windowSizeY), "Chess");

	// main loop that is active until the graphics window is closed
	while (window.isOpen())
	{
		// event that contains user interaction
		Event event;

		// check if there actually was some user interaction
		while (window.pollEvent(event))
		{
			// if window is closed
			if (event.type == Event::Closed)
			{
				window.close();
			}
			// if mousebutton is pressed
			else if (event.type == Event::MouseButtonPressed)
			{
				// if you are on the menu and click, reset the game and change the mode to game
				if (mode == 0)
				{
					Vector2i mouse_pos = sf::Mouse::getPosition(window);
					Vector2f translated_pos = window.mapPixelToCoords(mouse_pos);
					if(playLocalSprite.getGlobalBounds().contains(translated_pos))
					{
						mode = 1;
						resetGame();
					}
				}
				// if you are in the game, update it every time you click and change to game_over mode if the game ended (gamestate != 0)
				else if (mode == 1)
				{
					updateGame(event, window);
					
					if (gameState != 0)
					{
						mode = 2;
					}
				}
				// if your are on the game_over screen and click, change the mode to menu
				else if (mode == 2)
				{
					mode = 0;
				}
			}
		}

		// clear the screen of the graphics window
		window.clear(backgroundColor);
		
		// draw every thing for the current mode
		if (mode == 0)
		{
			drawMenu(window);
		}
		else if (mode == 1)
		{
			drawGame(window);
		}
		else if (mode == 2)
		{
			drawGameOver(window);
		}

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}