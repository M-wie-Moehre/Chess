// include SFML library headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

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

	// create a listener to wait for incoming connections on port 55001
    TcpListener listener;
    listener.listen(55001);

    // wait for a connection
    TcpSocket socket;
    listener.accept(socket);
	// print with which ip adress you are connected
    cout << "New client connected: " << socket.getRemoteAddress() << endl;

	// set blocking to false, so that the program doesn't stop and wait until something happen (for example until a message is received)
    socket.setBlocking(false);

    // create and open a new graphics window of size 800x800 Pixel and a title
	RenderWindow window(VideoMode(windowSizeX, windowSizeY), "Online Chess - Host");

	// main loop that is active until the graphics window is closed
	while (window.isOpen())
	{
		// event that contains user interaction
		Event event;

		// check if there actually was some user interaction
		while (window.pollEvent(event))
		{
			// window closed
			if (event.type == Event::Closed)
			{
				// close this and the other window
				socket.setBlocking(true);

				Packet packetSend;
				messageSend = 0;
				packetSend << messageSend;

				socket.send(packetSend);

				socket.setBlocking(false);

				window.close();

			}
			else if (event.type == Event::MouseButtonPressed)
			{
				// if you are on the menu and click, reset the game and change the mode to game
				if (mode == 0)
				{
					// send to the client, if the client is ready
					hostReady = !hostReady;

					socket.setBlocking(true);

					Packet packetSend;
					messageSend = hostReady ? 1 : 2;
					packetSend << messageSend;

					socket.send(packetSend);

					socket.setBlocking(false);

					if (hostReady && clientReady)
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

		Packet packetReceive;
        // receive a message from the client
        if (socket.receive(packetReceive) == Socket::Done)
        {
			Uint8 messageReceive;
            // if the message gets unpacked correctly
            if (packetReceive >> messageReceive)
            {
				if (messageReceive == 0)
				{
					window.close();
				}
				else if (mode == 0 && messageReceive == 1)
				{
                	clientReady = true;
				}
				else if (mode == 0)
				{
					clientReady = false;
				}

				if (hostReady && clientReady)
				{
					mode = 1;
					resetGame();
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