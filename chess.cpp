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
	/*
		load all assets
	*/
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
				// if you play online, send a disconnect to the other PC
				if (playOnline)
				{
					Packet packet;

					Uint8 status;
					status = 4; // status 4 = disconnect

					packet << status;

					socket.send(packet);
				}

				// close the window
				window.close();
			}
			// if mousebutton is pressed
			else if (event.type == Event::MouseButtonPressed)
			{
				// if you are on the menu screen check if you either press play local or play online
				if (mode == MENU)
				{
					// get the mouse position to check which button was pressed
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);

					// if the play local button is pressed
					if (playLocalSprite.getGlobalBounds().contains(translatedPosition))
					{
						// set the mode to game and reset the game
						mode = GAME;
						resetGame();
					}
					// if the play online button is pressed
					else if (playOnlineSprite.getGlobalBounds().contains(translatedPosition))
					{
						// set the mode to choose if you want to create a game (be host) or you want to join a game (be client)
						mode = CHOOSE_ONLINE_MODE;
						updateChooseOnlineMode(); // used to update the own ip address, that is displayed
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
				// if you are on the game_over screen check if you click the play again or back button
				else if (mode == GAME_OVER)
				{
					// get the mouse position
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);

					// if play again was clicked
					if (playAgainSprite.getGlobalBounds().contains(translatedPosition))
					{
						// if you play online, send over, that you are ready to play again
						if (playOnline)
						{
							Packet packet;

							Uint8 status;

							if (youAreHost)
							{
								hostReady = !hostReady;

								status = hostReady ? 2 : 3; 
								// to different status codes are used to send either that you are ready or not, to prevent that if one
								// message with a status isn't send over correctly, both the host and client still know whos ready
								// and who is not

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

						// if you play alone or play online and both players are ready, set the mode to game and reset the game
						if (!playOnline || (playOnline && hostReady && clientReady))
						{
							mode = GAME;
							resetGame();
							hostReady = false;
							clientReady = false;
						}
					}
					// if you click the back button
					else if (backSprite.getGlobalBounds().contains(translatedPosition))
					{
						// set the mode to menu
						mode = MENU;

						// if you play online, send a disconnect to the other PC
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
				// if you have to choose the online mode, so you either want to create a game (be host) or join a game (be client)
				else if (mode == CHOOSE_ONLINE_MODE)
				{
					// get mouse position
					Vector2i mousePosition = sf::Mouse::getPosition(window);
					Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
					
					// if you create a game
					if (createGameSprite.getGlobalBounds().contains(translatedPosition))
					{
						// set the listener to listen and non-blocking
						listener.listen(55001);
						listener.setBlocking(false);

						// used to loop the listener every frame later in the code 
						// (since it is set to non-blocking) to connect to the other PC
						creatingGame = !creatingGame;
					}
					// if you join a game
					else if (joinGameSprite.getGlobalBounds().contains(translatedPosition))
					{
						// if you entered a valid IP address
						if (isIpAddressValid())
						{
							// create the IP address
							IpAddress ipAdressInput(ipAdressInputString);

							// connect to the other PC (this function waits one second, if no connection could be made, stop searching)
							if (socket.connect(ipAdressInput, 55001, milliseconds(1000)) == Socket::Done)
							{
								cout << "Connected to host: " << ipAdressInput << "." << endl;

								// set the socket to non-blocking, since it is not used when playing the game and sending the pieces over
								socket.setBlocking(false);

								// set the mode to game, since both players are ready
								mode = GAME;
								resetGame();

								playOnline = true;
								youAreHost = false;

								// rename the window
								window.setTitle("Chess - Client");
							}
							else
							{
								cout << "Failed to connect to host." << endl;
							}
						}
						else
						{
							cout << "Entered invalid IP address." << endl;
						}
					}
					// if the back button is pressed
					else if (backSprite.getGlobalBounds().contains(translatedPosition))
					{
						// set the mode to menu
						mode = MENU;
					}
					// if the text with the own IP address is clicked
					else if (ipAdressText.getGlobalBounds().contains(translatedPosition))
					{
						// copy the IP address to the clipboard
						Clipboard::setString(ipAdressString);
						cout << "Copied IP address." << endl;
					}
					// if the text with the other IP address
					else if (ipAdressInputText.getGlobalBounds().contains(translatedPosition))
					{
						// paste the clipboard into the game
						ipAdressInputString = Clipboard::getString();
						cout << "Pasted IP address." << endl;
					}
				}
			}
			// if something was typed on the keyboard
			else if (event.type == Event::TextEntered)
			{
				// and you are on the choose online mode
				if (mode == CHOOSE_ONLINE_MODE)
				{
					// and the input is valid
					if (isprint(event.text.unicode))
					{
						// write the input to the IP address input
                    	ipAdressInputString += event.text.unicode;
					}
				}
			}
			// if backspace was pressed
			else if (event.type == Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					// and you are on the choose online mode
					if (mode == CHOOSE_ONLINE_MODE)
					{
						// and the IP address input isn't empty
						if (!ipAdressInputString.empty())
						{
							// remove the last character
							ipAdressInputString.pop_back();
						}
					}
				}
			}
		}
		
		// if you are creating a game
		if(creatingGame)
		{
			// listen for an incomming connection, when one comes in
			if (listener.accept(socket) == Socket::Done)
			{
				cout << "New client connected: " << socket.getRemoteAddress() << "." << endl;

				// set the socket to non-blocking, since it is not used, when sending the pieces over in the game
				socket.setBlocking(false);

				// set the mode to game and reset the game, since both players are ready
				mode = GAME;
				resetGame();

				playOnline = true;
				youAreHost = true;

				// rename the window
				window.setTitle("Chess - Host");

				// close the listener, since it is not needed anymore
				listener.close();

				creatingGame = false;
			}
		}

		// if you play online, listen for incomming messages from the other PC
		if (playOnline)
		{
			Packet packetReceive;
			// receive a message from the client
			if (socket.receive(packetReceive) == Socket::Done)
			{
				Uint8 status;
				// unpack the packet
				packetReceive >> status;

				// if status 1 = game information received
				if (status == 1)
				{
					// unpack the pieces
					for (int x = 0; x < 8; x++)
					{
						for (int y = 0; y < 8; y++)
						{
							packetReceive >> pieces[y][x];
						}
					}

					// unpack the en passant information
					packetReceive >> whiteEnPassant >> blackEnPassant;

					// unpack the pieces by pawnpromotion
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
				// if status 2 = other player is ready
				else if (status == 2)
				{
					// if you are host, set client to ready
					if (youAreHost)
					{
						clientReady = true;

						cout << "Client ready received." << endl;
					}
					// else if you are client, set host to ready
					else
					{
						hostReady = true;

						cout << "Host ready received." << endl;
					}

					// if both are ready, start the game
					if (playOnline && hostReady && clientReady)
					{
						mode = GAME;
						resetGame();
						hostReady = false;
						clientReady = false;
					}
				}
				// if status 3 = other player not ready
				else if (status == 3)
				{
					// if you are host, set client to not ready
					if (youAreHost)
					{
						clientReady = false;

						cout << "Client not ready received." << endl;
					}
					// else if you client, set host to not ready
					else
					{
						hostReady = false;

						cout << "Host not ready received." << endl;
					}
				}
				// if status 4 = disconnect received
				else if (status == 4)
				{
					// disconnect the socket
					socket.disconnect();
					socket.setBlocking(true);
					playOnline = false;

					// go back to the menu
					mode = MENU;

					cout << "Disconnect received." << endl;

					// rename the window
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