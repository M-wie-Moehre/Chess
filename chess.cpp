// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

int main()
{
	// local variables
	Color backgroundColor(25, 51, 45);

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

		// display everything you have drawn at once
		window.display();
	}

	return 0;
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