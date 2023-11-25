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
	// load all textures
	loadFonts();

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
			switch (event.type)
			{
				// window closed
				case Event::Closed:
					window.close();
					break;

				// mousebutton pressed
				case Event::MouseButtonPressed:
					if (mode == 0)
					{
						mode = 1;
						resetGame();
					}
					else if (mode == 1)
					{
						updateGame(event, window);
						
						if (gameState != 0)
						{
							mode = 2;
						}
					}
					else if (mode == 2)
					{
						mode = 0;
					}
					break;
					
				// we don't process other types of events
				default:
					break;
			}
		}

		// clear the screen of the graphics window
		window.clear(backgroundColor);
		
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
			drawGameOver(window, gameState);
		}

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}