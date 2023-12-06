// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

// include the custom game headers
#include "main.h"
#include "menu.h"
#include "choose_online_mode.h"
#include "game.h"
#include "game_over.h"

int main()
{
	// load all assets
	// load all fonts
	loadFonts();

	// load all textures
	loadMainTextures();

	loadMenuTextures();

	loadChooseOnlineModeTextures();

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
				if (playOnline)
				{
					Packet packet;

					Uint8 status;
					status = 4;

					packet << status;

					socket.send(packet);
				}

				window.close();
			}
			// if mousebutton is pressed
			else if (event.type == Event::MouseButtonPressed)
			{
				// if you are on the menu and click, reset the game and change the mode to game
				if (mode == MENU)
				{
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
					if (playLocalSprite.getGlobalBounds().contains(translatedPosition))
					{
						mode = GAME;
						resetGame();
					}
					else if (playOnlineSprite.getGlobalBounds().contains(translatedPosition))
					{
						mode = CHOOSE_ONLINE_MODE;
						updateChooseOnlineMode();
					}
				}
				// if you are in the game, update it every time you click and change to game_over mode if the game ended (gamestate != 0)
				else if (mode == GAME)
				{
					updateGame(event, window);
					
					if (gameState != 0)
					{
						mode = GAME_OVER;
					}
				}
				// if you are on the game_over screen and click, change the mode to menu
				else if (mode == GAME_OVER)
				{
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
					if (playAgainSprite.getGlobalBounds().contains(translatedPosition))
					{
						if (playOnline)
						{
							Packet packet;

							Uint8 status;

							if (youAreHost)
							{
								hostReady = !hostReady;

								status = hostReady ? 2 : 3;

								string message = hostReady ? "Host ready send." : "Host not ready send.";
								cout << message << endl;
							}
							else
							{
								clientReady = !clientReady;

								status = clientReady ? 2 : 3;

								string message = clientReady ? "Client ready send." : "Client not ready send.";
								cout << message << endl;
							}

							packet << status;

							socket.send(packet);
						}

						if (!playOnline || (playOnline && hostReady && clientReady))
						{
							mode = GAME;
							resetGame();
							hostReady = false;
							clientReady = false;
						}
					}
					else if (backSprite.getGlobalBounds().contains(translatedPosition))
					{
						mode = MENU;

						if (playOnline)
						{
							Packet packet;

							Uint8 status;
							status = 4;

							packet << status;

							socket.send(packet);

							socket.disconnect();
							socket.setBlocking(true);
							playOnline = false;

							cout << "Disconnect send." << endl;

							window.setTitle("Chess");
						}
					}
				}
				// if you have to choose the online mode
				else if (mode == CHOOSE_ONLINE_MODE)
				{
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
					if (createGameSprite.getGlobalBounds().contains(translatedPosition))
					{
						TcpListener listener;
						listener.listen(55001);

						if (listener.accept(socket) == Socket::Done)
						{
							cout << "New client connected: " << socket.getRemoteAddress() << endl;
							socket.setBlocking(false);

							mode = GAME;
							resetGame();

							playOnline = true;
							youAreHost = true;

							window.setTitle("Chess - Host");

							listener.close();
						}
						else
						{
							cout << "Failed to connect to client." << endl;
						}
					}
					else if (joinGameSprite.getGlobalBounds().contains(translatedPosition))
					{
						IpAddress ipAdress(ipAdressInput);

						cout << ipAdress << endl;
						
						if (ipAdress != IpAddress::None)
						{
							if (socket.connect(ipAdress, 55001) == Socket::Done)
							{
								cout << "Connected to host: " << ipAdress << endl;

								socket.setBlocking(false);

								mode = GAME;
								resetGame();

								playOnline = true;
								youAreHost = false;

								window.setTitle("Chess - Client");
							}
							else
							{
								cout << "Failed to connect to host." << endl;
							}
						}
						else
						{
							cout << "Invalid IP address entered." << endl;
						}
					}
					else if (backSprite.getGlobalBounds().contains(translatedPosition))
					{
						mode = MENU;
					}
				}
			}
			else if (event.type == Event::TextEntered)
			{
				if (mode == CHOOSE_ONLINE_MODE)
				{
					if (isprint(event.text.unicode))
					{
                    	ipAdressInput += event.text.unicode;
					}
				}
			}
			else if (event.type == Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (mode == CHOOSE_ONLINE_MODE)
					{
						if (!ipAdressInput.empty())
						{
							ipAdressInput.pop_back();
						}
					}
				}
			}
		}

		if (playOnline)
		{
			Packet packetReceive;
			// receive a message from the client
			if (socket.receive(packetReceive) == Socket::Done)
			{
				Uint8 status;

				packetReceive >> status;

				if (status == 1)
				{
					for (int x = 0; x < 8; x++)
					{
						for (int y = 0; y < 8; y++)
						{
							packetReceive >> pieces[y][x];
						}
					}

					packetReceive >> whiteEnPassant >> blackEnPassant;

					for (int x = 0; x < 8; x++)
					{
						for (int y = 0; y < 8; y++)
						{
							packetReceive >> piecesByPawnPromotion[y][x];
						}
					}

					// when you are host, you are the one to move next and otherwise, the client (black) is to move
					whiteToMove = youAreHost;

					// update the list of beaten pieces
					updateBeatenPieces();

					// check for checkmate and stalemate, to end the game
					if (isCheckmate())
					{
						gameState = whiteToMove ? 2 : 1;
					}
					else if (isStalemate())
					{
						gameState = 3;
					}

					if (gameState != 0)
					{
						mode = GAME_OVER;
					}

					cout << "Pieces received." << endl;
				}
				else if (status == 2)
				{
					if (youAreHost)
					{
						clientReady = true;

						cout << "Client ready received." << endl;
					}
					else
					{
						hostReady = true;

						cout << "Host ready received." << endl;
					}

					if (playOnline && hostReady && clientReady)
					{
						mode = GAME;
						resetGame();
						hostReady = false;
						clientReady = false;
					}
				}
				else if (status == 3)
				{
					if (youAreHost)
					{
						clientReady = false;

						cout << "Client not ready received." << endl;
					}
					else
					{
						hostReady = false;

						cout << "Host not ready received." << endl;
					}
				}
				else if (status == 4)
				{
					socket.disconnect();
					socket.setBlocking(true);
					playOnline = false;
					mode = MENU;

					cout << "Disconnect received." << endl;
					window.setTitle("Chess");
				}
			}
		}

		// clear the screen of the graphics window
		window.clear(backgroundColor);
		
		// draw every thing for the current mode
		if (mode == MENU)
		{
			drawMenu(window);
		}
		else if (mode == GAME)
		{
			drawGame(window);
		}
		else if (mode == GAME_OVER)
		{
			drawGameOver(window);
		}
		else if (mode == CHOOSE_ONLINE_MODE)
		{
			drawChooseOnlineMode(window);
		}

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}