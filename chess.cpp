// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

int main()
{
	// local variables and funcitons
	Color backgroundColor(25, 51, 45);

	// start position of all pieces (0 = empty, 1 = pawn, 2 = rook, 3 = knight, 4 = bishop, 5 = queen, 6 = king, for white += 6)
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

	void drawPieces(RenderWindow& window, int pieces[8][8], Sprite pieceSprites[12]);

	// textures and sprites
	Texture boardTexture;
	Sprite boardSprite;

	Texture boardShadowTexture;
	Sprite boardShadowSprite;

	Texture pieceTextures[12];
	Sprite pieceSprites[12];
	string pieceNames[6] = {"pawn", "rook", "knight", "bishop", "queen", "king"};

	int loadPieceTextures(Texture textures[12], Sprite sprites[12], string names[6]);

	if (!boardTexture.loadFromFile("textures\\board.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardSprite.setTexture(boardTexture);
	boardSprite.setScale(5, 5); // one pixel on the images is equal to five pixel on the window
	boardSprite.setPosition(80, 80);

	if (!boardShadowTexture.loadFromFile("textures\\board_shadow.png"))
	{
		cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
		return 1;
	}

	boardShadowSprite.setTexture(boardShadowTexture);
	boardShadowSprite.setScale(5, 5);
	boardShadowSprite.setPosition(95, 95); // shift 3 pixels on the image scale further

	loadPieceTextures(pieceTextures, pieceSprites, pieceNames);

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
			// react on the request to close the graphics window
			if (event.type == Event::Closed)
				window.close();
		}

		// clear the screen of the graphics window
		window.clear(backgroundColor);

		// draw board
		window.draw(boardShadowSprite, BlendMultiply);
		window.draw(boardSprite);

		drawPieces(window, pieces, pieceSprites);

		// display everything you have drawn at once
		window.display();
	}

	return 0;
}

void drawPieces(RenderWindow& window, int pieces[8][8], Sprite pieceSprites[12])
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

			// set position for current piece
			pieceSprites[pieces[y][x] - 1].setPosition(100 + x * 16 * 5, 80 + y * 16 * 5);

			// adjust position for certain pieces, so that all line up
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

			window.draw(pieceSprites[pieces[y][x] - 1]);
		}
	}
}

int loadPieceTextures(Texture textures[12], Sprite sprites[12], string names[6])
{
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
	
	return 0;
}