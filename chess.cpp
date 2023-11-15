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

	void drawPieces(RenderWindow& window, int pieces[8][8], Sprite pieceSprites[12], Sprite pieceShadowSprites[4], 
	Vector2i mousePos, Vector2i pickedUpPiece, bool piecePickedUp);

	// textures and sprites
	Texture boardTexture;
	Sprite boardSprite;

	Texture boardShadowTexture;
	Sprite boardShadowSprite;

	Texture pieceTextures[12];
	Sprite pieceSprites[12];
	Texture pieceShadowsTextures[4];
	Sprite pieceShadowsSprites[4];
	string pieceNames[6] = {"pawn", "rook", "knight", "bishop", "queen", "king"}; // required to load in the textures automatically

	int loadPieceTextures(Texture textures[12], Sprite sprites[12], Texture pieceShadowsTextures[4], Sprite pieceShadowsSprites[4], string names[6]);

	if (!boardTexture.loadFromFile("textures\\board.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardSprite.setTexture(boardTexture);
	boardSprite.setScale(5, 5); // one pixel on the images is equal to five pixel on the window
	boardSprite.setPosition(65, 65);

	if (!boardShadowTexture.loadFromFile("textures\\board_shadow.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardShadowSprite.setTexture(boardShadowTexture);
	boardShadowSprite.setScale(5, 5);
	boardShadowSprite.setPosition(80, 80); // shift 3 pixels on the image scale further

	loadPieceTextures(pieceTextures, pieceSprites, pieceShadowsTextures, pieceShadowsSprites, pieceNames);

	Vector2i mousePos = {-1, -1};
	Vector2i pickedUpPiece = {-1, -1}; // which piece is picked up (no piece = {-1, -1})
	bool piecePickedUp = false; // is a piece picked up

    // create and open a new graphics window of size 800x800 Pixel and a title
	RenderWindow window(VideoMode(800, 800), "Chess");

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
						if (Mouse::getPosition(window).x - 80 < 0 || Mouse::getPosition(window).y - 80 < 0 || 
						Mouse::getPosition(window).x - 80 > 16 * 8 * 5 || Mouse::getPosition(window).y - 80 > 16 * 8 * 5)
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
							mousePos = {(Mouse::getPosition(window).x - 80) / 80, (Mouse::getPosition(window).y - 80) / 80};

							// if no piece is picked up and at the mouse position is a piece, pick it up
							if (!piecePickedUp && pieces[mousePos.y][mousePos.x] != 0)
							{
								pickedUpPiece = mousePos;
								piecePickedUp = true;
							}
							// if you place a picked up piece back at the same spot
							else if (piecePickedUp && pickedUpPiece == mousePos)
							{
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

		// draw pieces
		drawPieces(window, pieces, pieceSprites, pieceShadowsSprites, mousePos, pickedUpPiece, piecePickedUp);

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}

// function to draw all pieces
void drawPieces(RenderWindow& window, int pieces[8][8], Sprite pieceSprites[12], Sprite pieceShadowSprites[4], Vector2i mousePos, 
Vector2i pickedUpPiece, bool piecePickedUp)
{
	// go trough every piece
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			// skip empty fields
			if (pieces[y][x] == 0)
			{
				continue;
			}

			// determin which shadow is used for the current piece
			int currentShadowIndex = 3;
			for (int i = 0; i < 3; i++)
			{
				if (pieces[y][x] - 1 == i || pieces[y][x] - 1 == i + 6)
				{
					currentShadowIndex = i;
				}
			}
			
			
			// position picked up pieces at the mouse postion
			if (piecePickedUp && pickedUpPiece.x == x && pickedUpPiece.y == y)
			{
				pieceSprites[pieces[y][x] - 1].setPosition(Mouse::getPosition(window).x - 20, Mouse::getPosition(window).y - 30);

				pieceShadowSprites[currentShadowIndex].setPosition(Mouse::getPosition(window).x - 10, Mouse::getPosition(window).y + 10);
			}
			// position other pieces on the board
			else
			{
				pieceSprites[pieces[y][x] - 1].setPosition(100 + x * 16 * 5, 80 + y * 16 * 5);

				pieceShadowSprites[currentShadowIndex].setPosition(100 + x * 16 * 5 + 10, 80 + y * 16 * 5 + 40);
			}

			// adjust position for certain pieces, so that all line up (because some pieces have different heights)
			if (pieces[y][x] == 2 || pieces[y][x] == 8)
			{
				Vector2 position = pieceSprites[pieces[y][x] - 1].getPosition();
				position.y -= 5;
				pieceSprites[pieces[y][x] - 1].setPosition(position);
			}
			else if (pieces[y][x] == 3 || pieces[y][x] == 9)
			{
				Vector2 position = pieceSprites[pieces[y][x] - 1].getPosition();
				position.y -= 10;
				pieceSprites[pieces[y][x] - 1].setPosition(position);
			}
			else if (pieces[y][x] != 1 && pieces[y][x] != 7)
			{
				Vector2 position = pieceSprites[pieces[y][x] - 1].getPosition();
				position.y -= 15;
				pieceSprites[pieces[y][x] - 1].setPosition(position);
			}

			// draw the piece
			window.draw(pieceSprites[pieces[y][x] - 1]);
			window.draw(pieceShadowSprites[currentShadowIndex], BlendMultiply);
		}
	}
}

// function to load all textures for the pieces
int loadPieceTextures(Texture textures[12], Sprite sprites[12], Texture pieceShadowsTextures[4], Sprite pieceShadowsSprites[4], string names[6])
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
		sprites[i].setScale(5, 5); // one pixel on the images is equal to five pixel on the window
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
		sprites[i + 6].setScale(5, 5); // one pixel on the images is equal to five pixel on the window
	}

	for (int i = 0; i < 4; i++)
	{
		if (!pieceShadowsTextures[i].loadFromFile("textures\\" + to_string(i + 1) + "_piece_shadow.png"))
		{
			cout << "Couldn't load texture \"textures\\" + to_string(i + 1) + "_piece_shadow.png\". Exiting.." << endl;
			return 1;
		}

		pieceShadowsSprites[i].setTexture(pieceShadowsTextures[i]);
		pieceShadowsSprites[i].setScale(5, 5); // one pixel on the images is equal to five pixel on the window
	}
	
	
	return 0;
}