// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

int main()
{
	// local variables and functions
	Color backgroundColor(25, 51, 45);
	int pixelScale = 4; // the factor, on how much the textures get scaled up
	int windowSizeX = 800;
	int windowSizeY = 800;

	// start position of all pieces (0 = empty, 1 = pawn, 2 = rook, 3 = knight, 4 = bishop, 5 = queen, 6 = king, for black += 6)
	int pieces[8][8] = {
		{8, 9, 10, 11, 12, 10, 9, 8},
		{7, 7, 7, 7, 7, 7, 7, 7},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{2, 3, 4, 5, 6, 4, 3, 2}
	};
	int beatenPieces[12] = {0}; // saves how many pieces of each type got beaten

	void updateBeatenPieces(int pieces[8][8], int (&beatenPieces)[12]); // function to update the list of beaten pieces

	// function to draw all pieces
	void drawPieces(RenderWindow &window, int pixelScale, Vector2i boardPosition, int pieces[8][8], int beatenPieces[12], Sprite (&pieceSprites)[12], 
	Sprite pieceShadowSprites[4], Vector2i mousePos, Vector2i pickedUpPiece, bool piecePickedUp);

	// textures and sprites
	Texture pieceTextures[12];
	Sprite pieceSprites[12];
	Texture pieceShadowsTextures[4];
	Sprite pieceShadowsSprites[4];
	string pieceNames[6] = {"pawn", "rook", "knight", "bishop", "queen", "king"}; // required to load in the textures automatically

	// function to load all textures for the pieces
	int loadPieceTextures(int pixelScale, Texture textures[12], Sprite sprites[12], Texture pieceShadowsTextures[4], Sprite pieceShadowsSprites[4], 
	string names[6]);

	Texture boardTexture;
	Sprite boardSprite;
	Vector2i boardPosition = {(windowSizeX - 134 * pixelScale) / 2, (windowSizeY - 134 * pixelScale) / 2};

	// load the board texture
	if (!boardTexture.loadFromFile("textures\\board.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardSprite.setTexture(boardTexture);
	boardSprite.setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	boardSprite.setPosition(boardPosition.x, boardPosition.y);

	Texture boardShadowTexture;
	Sprite boardShadowSprite;

	// load the texture of the boards shadow
	if (!boardShadowTexture.loadFromFile("textures\\board_shadow.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardShadowSprite.setTexture(boardShadowTexture);
	boardShadowSprite.setScale(pixelScale, pixelScale);
	boardShadowSprite.setPosition(boardPosition.x + 3 * pixelScale, boardPosition.y + 3 * pixelScale); // shift 3 pixels on the image scale further

	loadPieceTextures(pixelScale, pieceTextures, pieceSprites, pieceShadowsTextures, pieceShadowsSprites, pieceNames);

	Vector2i mousePos = {-1, -1}; // saves, which field the mouse clicked on
	Vector2i pickedUpPiece = {-1, -1}; // which piece is picked up (no piece = {-1, -1})
	bool piecePickedUp = false; // is a piece picked up

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
					if (event.mouseButton.button == Mouse::Left) // specifies left mousebutton
					{
						// if mouse isn't on the board
						if (Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale < 0 || Mouse::getPosition(window).y - boardPosition.y - 3 * 
						pixelScale < 0 || Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale > 16 * 8 * pixelScale || 
						Mouse::getPosition(window).y - boardPosition.y - 3 * pixelScale > 16 * 8 * pixelScale)
						{
							mousePos = {-1, -1};

							// put piece back, when one is picked up
							if (piecePickedUp)
							{
								pickedUpPiece = {-1, -1};
								piecePickedUp =  false;
							}
						}
						// when mouse is on the board
						else
						{
							// calculate mouse position on the board
							mousePos = {(Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale) / (16 * pixelScale), 
							(Mouse::getPosition(window).y - boardPosition.y - 3 * pixelScale) / (16 * pixelScale)};

							// if no piece is picked up and at the mouse position is a piece
							if (!piecePickedUp && pieces[mousePos.y][mousePos.x] != 0)
							{
								// pick the piece up
								pickedUpPiece = mousePos;
								piecePickedUp = true;
							}
							// if you place a picked up piece back at the same spot
							else if (piecePickedUp && pickedUpPiece == mousePos)
							{
								// put the piece down, without changing anything
								pickedUpPiece = {-1, -1};
								piecePickedUp = false;
							}
							// if you place the picked up piece on an empty field
							else if (piecePickedUp && pieces[mousePos.y][mousePos.x] == 0)
							{
								// switch the pieces (empty <-> pickedUpPiece)
								pieces[mousePos.y][mousePos.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
								pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;

								pickedUpPiece = {-1, -1};
								piecePickedUp = false;
							}
							// if you place the piece on another piece
							else if (piecePickedUp && pieces[mousePos.y][mousePos.x] != 0 && ((pieces[mousePos.y][mousePos.x] <= 6 && 
							pieces[pickedUpPiece.y][pickedUpPiece.x] >= 7) || (pieces[mousePos.y][mousePos.x] >= 7 && 
							pieces[pickedUpPiece.y][pickedUpPiece.x] <= 6)))
							{
								// switch the pieces (empty <-> pickedUpPiece)
								pieces[mousePos.y][mousePos.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
								pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;

								pickedUpPiece = {-1, -1};
								piecePickedUp = false;

								updateBeatenPieces(pieces, beatenPieces);
							}
						}
					}
					break;
					
				// we don't process other types of events
				default:
					break;
			}
		}

		// clear the screen of the graphics window
		window.clear(backgroundColor);

		// draw board
		window.draw(boardShadowSprite, BlendMultiply);
		window.draw(boardSprite);

		// draw all pieces
		drawPieces(window, pixelScale, boardPosition, pieces, beatenPieces, pieceSprites, pieceShadowsSprites, mousePos, pickedUpPiece, piecePickedUp);

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}

// function to draw one piece at a position
void drawPiece(RenderWindow &window, int piece, Vector2i position, Sprite pieceSprites[12], Sprite pieceShadowSprites[4], int pixelScale)
{
	// set position for the piece
	pieceSprites[piece].setPosition(position.x, position.y);

	// adjust some piece positions, so that all line up (because some pieces are taller)
	if (piece == 1 || piece == 7)
	{
		Vector2 position = pieceSprites[piece].getPosition();
		position.y -= 1 * pixelScale;
		pieceSprites[piece].setPosition(position);
	}
	else if (piece == 2 || piece == 8)
	{
		Vector2 position = pieceSprites[piece].getPosition();
		position.y -= 2 * pixelScale;
		pieceSprites[piece].setPosition(position);
	}
	else if (piece != 0 && piece != 6)
	{
		Vector2 position = pieceSprites[piece].getPosition();
		position.y -= 3 * pixelScale;
		pieceSprites[piece].setPosition(position);
	}

	// get the shadow for the current piece
	int currentShadowIndex = 3;
	for (int i = 0; i < 3; i++)
	{
		if (piece == i || piece == i + 6)
		{
			currentShadowIndex = i;
		}
	}

	// set the shadows position
	pieceShadowSprites[currentShadowIndex].setPosition(position.x + 2 * pixelScale, position.y + 8 * pixelScale);

	// draw piece and shadow
	window.draw(pieceSprites[piece]);
	window.draw(pieceShadowSprites[currentShadowIndex], BlendMultiply);
}

// function to draw all pieces
void drawPieces(RenderWindow &window, int pixelScale, Vector2i boardPosition, int pieces[8][8], int beatenPieces[12], Sprite (&pieceSprites)[12], 
Sprite pieceShadowSprites[4], Vector2i mousePos, Vector2i pickedUpPiece, bool piecePickedUp)
{
	/*
		draw pieces, that are still on the board
	*/
	// go trough every field
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			// skip empty fields
			if (pieces[y][x] == 0)
			{
				continue;
			}
			
			// draw picked up pieces at the mouse postion
			if (piecePickedUp && pickedUpPiece.x == x && pickedUpPiece.y == y)
			{
				Vector2i position = {Mouse::getPosition(window).x - 4 * pixelScale, Mouse::getPosition(window).y - 6 * pixelScale};
				drawPiece(window, pieces[y][x] - 1, position, pieceSprites, pieceShadowSprites, pixelScale);
			}
			// draw other pieces on the board
			else
			{
				Vector2i position = {boardPosition.x + 7 * pixelScale + x * 16 * 4, boardPosition.x + 3 * pixelScale + y * 16 * 4};
				drawPiece(window, pieces[y][x] - 1, position, pieceSprites, pieceShadowSprites, pixelScale);
			}
		}
	}

	/*
		draw beaten pieces
	*/
	int offset = 0; // saves how much the current piece needs to be shiftet on the x-axis
	// go trough the list of beaten pieces
	// go trough white pieces first
	for (int i = 0; i < 6; i++)
	{
		bool addOffset = false;

		// go trough every piece of one type
		for (int j = 0; j < beatenPieces[i]; j++)
		{
			Vector2i position = {boardPosition.x + offset, 40};
			drawPiece(window, i, position, pieceSprites, pieceShadowSprites, pixelScale);
			offset += 3 * pixelScale; // increase offset only slightly
			addOffset = true;
		}

		// when a least one piece of the type got drawn, increase the offset for the next type
		if(addOffset)
		{
			offset += 10 * pixelScale;
		}
	}

	offset = 0; // restet offset for black pieces
	// go trough black pieces
	for (int i = 6; i < 12; i++)
	{
		bool addOffset = false;
		// go trough every piece of one type
		for (int j = 0; j < beatenPieces[i]; j++)
		{
			Vector2i position = {boardPosition.x + offset, 716};
			drawPiece(window, i, position, pieceSprites, pieceShadowSprites, pixelScale);
			offset += 3 * pixelScale; // increase offset only slightly
			addOffset = true;
		}

		// when a least one piece of the type got drawn, increase the offset for the next type
		if(addOffset)
		{
			offset += 10 * pixelScale;
		}
	}
}

// function to update the list of beaten pieces
void updateBeatenPieces(int pieces[8][8], int (&beatenPieces)[12])
{
	// go trough every possible piece
	for (int i = 0; i < 12; i++)
	{
		// reset piece count to the start count
		// for example white pawns to eight, since there were eight pawns at the start
		if (i == 0 || i == 6)
		{
			beatenPieces[i] = 8;
		}
		// here rooks, knights and bishops, both white and black get reset
		else if (i == 1 || i == 7 || i == 2 || i == 8 || i == 3 || i == 9)
		{
			beatenPieces[i] = 2;
		}
		// here the king and queen of white and black get reset
		else
		{
			beatenPieces[i] = 1;
		}
		
		// loop through every field and if there is a piece, remove 1 from its count -> you will end up with the amount of pieces beaten for each type
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (pieces[y][x] == i + 1)
				{
					beatenPieces[i] -= 1;
				}
			}
		}
	}
}

// function to load all textures for the pieces
int loadPieceTextures(int pixelScale, Texture textures[12], Sprite sprites[12], Texture pieceShadowsTextures[4], Sprite pieceShadowsSprites[4], string names[6])
{
	// load textures of white pieces
	for (int i = 0; i < 6; i++)
	{
		if (!textures[i].loadFromFile("textures\\" + names[i] + "_white.png"))
		{
			cout << "Couldn't load texture \"textures\\" + names[i] + "_white.png\". Exiting.." << endl;
			return 1;
		}

		sprites[i].setTexture(textures[i]);
		sprites[i].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}
	
	// load texture of black pieces
	for (int i = 0; i < 6; i++)
	{
		if (!textures[i + 6].loadFromFile("textures\\" + names[i] + "_black.png"))
		{
			cout << "Couldn't load texture \"textures\\" + names[i] + "_black.png\". Exiting.." << endl;
			return 1;
		}

		sprites[i + 6].setTexture(textures[i + 6]);
		sprites[i + 6].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}

	for (int i = 0; i < 4; i++)
	{
		if (!pieceShadowsTextures[i].loadFromFile("textures\\" + to_string(i + 1) + "_piece_shadow.png"))
		{
			cout << "Couldn't load texture \"textures\\" + to_string(i + 1) + "_piece_shadow.png\". Exiting.." << endl;
			return 1;
		}

		pieceShadowsSprites[i].setTexture(pieceShadowsTextures[i]);
		pieceShadowsSprites[i].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}
	
	
	return 0;
}