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
	Color invalidFieldColor(130, 130, 130);
	int pixelScale = 4; // the factor, on how much the textures get scaled up
	int windowSizeX = 800;
	int windowSizeY = 800;

	// saves the position of all pieces on the board (0 = empty, 1 = white pawn, 2 = white rook, 3 = white knight, 4 = white bishop, 
	// 5 = white queen, 6 = white king, for black += 6)
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
	bool validPiecePositions[8][8];

	int whiteEnPassant = -1; // saves which white pawn moved two fields forward and can be beaten en passant (-1: none)
	int blackEnPassant = -1; // saves which black pawn moved two fields forward and can be beaten en passant (-1: none)

	// function to update the list of beaten pieces
	void updateBeatenPieces(int pieces[8][8], int (&beatenPieces)[12]);
	// function that updates the list of valid positions a piece can move to
	void updateValidPiecePositions(int piece, int pieces[8][8], Vector2i piecePosition, bool (&validPiecePositions)[8][8], 
	int whiteEnPassant, int blackEnPassant);

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
	bool whiteToMove = true; // is white player the next to move

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
								// if white is to move and the piece is white or black is to move and the piece is black, pick the piece up
								if ((whiteToMove && pieces[mousePos.y][mousePos.x] - 1 >= 0 && pieces[mousePos.y][mousePos.x] - 1 <= 5) ||
								(!whiteToMove && pieces[mousePos.y][mousePos.x] - 1 >= 6 && pieces[mousePos.y][mousePos.x] - 1 <= 11))
								{
									pickedUpPiece = mousePos;
									piecePickedUp = true;
									updateValidPiecePositions(pieces[pickedUpPiece.y][pickedUpPiece.x] - 1, pieces, pickedUpPiece, validPiecePositions, 
									whiteEnPassant, blackEnPassant);
								}
							}
							// if you place the picked up piece on a valid field
							else if (piecePickedUp && validPiecePositions[mousePos.y][mousePos.x])
							{
								// if you put the piece not back at the same spot
								if (pickedUpPiece != mousePos)
								{
									// if a white pawn is moved two fields forward
									if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 0 && pickedUpPiece.y - 2 == mousePos.y)
									{
										whiteEnPassant = pickedUpPiece.x;
									}
									else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 6 && pickedUpPiece.y + 2 == mousePos.y)
									{
										blackEnPassant = pickedUpPiece.x;
									}
									else if (whiteToMove)
									{
										whiteEnPassant = -1;
									}
									else if (!whiteToMove)
									{
										blackEnPassant = -1;
									}

									if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 0 && pickedUpPiece.y == 3 && mousePos.x == blackEnPassant)
									{
										pieces[mousePos.y][mousePos.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
										pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
										pieces[mousePos.y + 1][mousePos.x] = 0;
									}
									else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 6 && pickedUpPiece.y == 4 && mousePos.x == whiteEnPassant)
									{
										pieces[mousePos.y][mousePos.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
										pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
										pieces[mousePos.y - 1][mousePos.x] = 0;
									}
									else
									{
										// switch the pieces (empty <-> pickedUpPiece)
										pieces[mousePos.y][mousePos.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
										pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
									}


									whiteToMove = !whiteToMove;
								}

								pickedUpPiece = {-1, -1};
								piecePickedUp = false;

								updateBeatenPieces(pieces, beatenPieces);
							}
						}
					}
					else if (event.mouseButton.button == Mouse::Right) // when right mouse button is pressed, put piece back
					{
						pickedUpPiece = {-1, -1};
						piecePickedUp =  false;
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

		// draw invalid fields (fields, the picked up piece can't move to)
		if (piecePickedUp)
		{
			// go through every field
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					// check if it is not valid
					if (!validPiecePositions[y][x])
					{
						// create a rectangle at the position of the field and draw it
						RectangleShape invalidField;
						invalidField.setFillColor(invalidFieldColor);
						invalidField.setSize(Vector2f(16 * pixelScale, 16 * pixelScale));
						invalidField.setPosition(boardPosition.x + 3 * pixelScale + x * 16 * pixelScale, boardPosition.y + 3 * pixelScale + y * 16 * pixelScale);

						window.draw(invalidField, BlendMultiply);
					}
				}
			}
		}

		// draw all pieces
		drawPieces(window, pixelScale, boardPosition, pieces, beatenPieces, pieceSprites, pieceShadowsSprites, mousePos, pickedUpPiece, piecePickedUp);

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}

// function that returns true if a piece would jump over another piece when moved
bool isPieceInWay(int pieces[8][8], Vector2i piecePosition, Vector2i mousePosition)
{
	// if the piece is not moved at all or just one field, return false, since there can be no piece in the way
	if (abs(piecePosition.x - mousePosition.x) < 2 && abs(piecePosition.y - mousePosition.y) < 2)
	{
		return false;
	}

	// if the piece is moved horizontal
	if (piecePosition.y == mousePosition.y)
	{
		// if the piece is moved to the right
		if (piecePosition.x < mousePosition.x)
		{
			// go through every field in between and check if there is a piece
			for (int x = piecePosition.x + 1; x < mousePosition.x; x++)
			{
				if (pieces[piecePosition.y][x] != 0)
				{
					return true;
				}
			}
			return false;
		}
		// if the piece is moved to the left
		else
		{
			for (int x = piecePosition.x - 1; x > mousePosition.x; x--)
			{
				if (pieces[piecePosition.y][x] != 0)
				{
					return true;
				}
			}
			return false;
		}		
	}
	// if the piece is moved vertical
	else if (piecePosition.x == mousePosition.x)
	{
		// if the piece is moved down
		if (piecePosition.y < mousePosition.y)
		{
			for (int y = piecePosition.y + 1; y < mousePosition.y; y++)
			{
				if (pieces[y][piecePosition.x] != 0)
				{
					return true;
				}
			}
			return false;
		}
		// if the piece is moved up
		else
		{
			for (int y = piecePosition.y - 1; y > mousePosition.y; y--)
			{
				if (pieces[y][piecePosition.x] != 0)
				{
					return true;
				}
			}
			return false;
		}		
	}
	// if the piece is moved diagonal
	else if(abs(piecePosition.x - mousePosition.x) == abs(piecePosition.y - mousePosition.y))
	{
		// get directions x: -1 = left, 1 = right; y: -1 = up, 1 = down
		int directionToMoveX = piecePosition.x > mousePosition.x ? -1 : 1;
		int directionToMoveY = piecePosition.y > mousePosition.y ? -1 : 1;

		// calculate how many steps the for loop needs to go through
		int stepsToMove = abs(piecePosition.x - mousePosition.x);

		// count up the steps
		for (int i = 1; i < stepsToMove; i++)
		{
			// add the step to the respectiv direction at both axis
			if (pieces[piecePosition.y + i * directionToMoveY][piecePosition.x + i * directionToMoveX] != 0)
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

// function that returns if the given piece can jump to the given position
bool canPieceMoveHere(int piece, int pieces[8][8], Vector2i piecePosition, Vector2i mousePosition, int whiteEnPassant, int blackEnPassant)
{
	/*
		general rules
	*/
	// if the piece is placed back at the same spot
	if (piecePosition == mousePosition)
	{
		return true;
	}

	// if the piece is placed on the same color
	if (((piece >= 0 && piece <= 5) && (pieces[mousePosition.y][mousePosition.x] - 1 >= 0 && pieces[mousePosition.y][mousePosition.x] - 1 <= 5)) ||
	((piece >= 6 && piece <= 12) && (pieces[mousePosition.y][mousePosition.x] - 1 >= 6 && pieces[mousePosition.y][mousePosition.x] - 1 <= 12)))
	{
		return false;
	}

	/*
		piece rules
	*/
	// go through every piece type
	if (piece == 0) // white pawn
	{
		if (piecePosition.y - 1 == mousePosition.y)
		{
			if (piecePosition.x == mousePosition.x && pieces[piecePosition.y - 1][piecePosition.x] == 0)
			{
				return true;
			}
			else if (piecePosition.x - 1 == mousePosition.x && ((pieces[piecePosition.y - 1][piecePosition.x - 1] - 1 >= 6 && 
			pieces[piecePosition.y - 1][piecePosition.x - 1] - 1 <= 11) || (mousePosition.x == blackEnPassant && piecePosition.y == 3)))
			{
				return true;
			}
			else if (piecePosition.x + 1 == mousePosition.x && ((pieces[piecePosition.y - 1][piecePosition.x + 1] - 1 >= 6 && 
			pieces[piecePosition.y - 1][piecePosition.x + 1] - 1 <= 11) || (mousePosition.x == blackEnPassant && piecePosition.y == 3)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (piecePosition.y - 2 == mousePosition.y)
		{
			if (piecePosition.x == mousePosition.x && piecePosition.y == 6 && pieces[piecePosition.y - 1][piecePosition.x] == 0 && 
			pieces[piecePosition.y - 2][piecePosition.x] == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (piece == 6) // black pawn
	{
		if (piecePosition.y + 1 == mousePosition.y)
		{
			if (piecePosition.x == mousePosition.x && pieces[piecePosition.y + 1][piecePosition.x] == 0)
			{
				return true;
			}
			else if (piecePosition.x - 1 == mousePosition.x && ((pieces[piecePosition.y + 1][piecePosition.x - 1] - 1 >= 0 && 
			pieces[piecePosition.y + 1][piecePosition.x - 1] - 1 <= 5) || (mousePosition.x == whiteEnPassant && piecePosition.y == 4)))
			{
				return true;
			}
			else if (piecePosition.x + 1 == mousePosition.x && ((pieces[piecePosition.y + 1][piecePosition.x + 1] - 1 >= 0 && 
			pieces[piecePosition.y + 1][piecePosition.x + 1] - 1 <= 5) || (mousePosition.x == whiteEnPassant && piecePosition.y == 4)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (piecePosition.y + 2 == mousePosition.y)
		{
			if (piecePosition.x == mousePosition.x && piecePosition.y == 1 && pieces[piecePosition.y + 1][piecePosition.x] == 0 && 
			pieces[piecePosition.y + 2][piecePosition.x] == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (piece == 1 || piece == 7) // black and white rook
	{
		// make sure it only moves verical and horizontal
		if (piecePosition.x != mousePosition.x && piecePosition.y != mousePosition.y)
		{
			return false;
		}
		else if (isPieceInWay(pieces, piecePosition, mousePosition))
		{
			return false;
		}
	}
	else if (piece == 2 || piece == 8) // black and white knight
	{
		// check all positions the knight could jump to
		if (!(piecePosition.x - 1 == mousePosition.x && piecePosition.y + 2 == mousePosition.y) &&
		!(piecePosition.x + 1 == mousePosition.x && piecePosition.y + 2 == mousePosition.y) &&
		!(piecePosition.x + 1 == mousePosition.x && piecePosition.y - 2 == mousePosition.y) &&
		!(piecePosition.x - 1 == mousePosition.x && piecePosition.y - 2 == mousePosition.y) &&
		!(piecePosition.x - 2 == mousePosition.x && piecePosition.y + 1 == mousePosition.y) &&
		!(piecePosition.x + 2 == mousePosition.x && piecePosition.y + 1 == mousePosition.y) &&
		!(piecePosition.x + 2 == mousePosition.x && piecePosition.y - 1 == mousePosition.y) &&
		!(piecePosition.x - 2 == mousePosition.x && piecePosition.y - 1 == mousePosition.y))
		{
			return false;
		}
	}
	else if (piece == 3 || piece == 9) // black and white bishop
	{
		// make sure it only moves diagonal
		if (abs(piecePosition.x - mousePosition.x) != abs(piecePosition.y - mousePosition.y))
		{
			return false;
		}
		else if (isPieceInWay(pieces, piecePosition, mousePosition))
		{
			return false;
		}
	}
	else if (piece == 4 || piece == 10) // black and white queen
	{
		// make sure it only moves horizontal, vertical and diagonal
		if (abs(piecePosition.x - mousePosition.x) != abs(piecePosition.y - mousePosition.y) && 
		piecePosition.x != mousePosition.x && piecePosition.y != mousePosition.y)
		{
			return false;
		}
		else if (isPieceInWay(pieces, piecePosition, mousePosition))
		{
			return false;
		}
	}
	else if (piece == 5 || piece == 11) // black and white king
	{
		// make sure it only moves one step
		if (abs(piecePosition.x - mousePosition.x) > 1 || abs(piecePosition.y - mousePosition.y) > 1)
		{
			return false;
		}
	}

	// else the piece can jump to the given position, so return true
	return true;
}

// function that updates the list of valid positions a piece can move to
void updateValidPiecePositions(int piece, int pieces[8][8], Vector2i piecePosition, bool (&validPiecePositions)[8][8], 
int whiteEnPassant, int blackEnPassant)
{
	// go through every field
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			// enter the x and y coordinate as the mouse position to check if the piece can move there
			validPiecePositions[y][x] = canPieceMoveHere(piece, pieces, piecePosition, Vector2i(x, y), whiteEnPassant, blackEnPassant);
		}
	}
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
	// go through every field
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
	// go through the list of beaten pieces
	// go through white pieces first
	for (int i = 0; i < 6; i++)
	{
		bool addOffset = false;

		// go through every piece of one type
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
	// go through black pieces
	for (int i = 6; i < 12; i++)
	{
		bool addOffset = false;
		// go through every piece of one type
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
	// go through every possible piece
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