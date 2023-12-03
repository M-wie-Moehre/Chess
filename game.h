#ifndef GAME_H
#define GAME_H

// include SFML library headers
#include <SFML/Graphics.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>



// game variables

Color invalidFieldColor(130, 130, 130);

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

int beatenPieces[12] = {0}; // saves how many pieces of each type got beaten (0 = white pawn, 1 = white rook ...)
bool validPiecePositions[8][8]; // saves, which fields, the current picked up piece can be put on

int whiteEnPassant = -1; // saves which white pawn moved two fields forward and can be beaten en passant (-1: none)
int blackEnPassant = -1; // saves which black pawn moved two fields forward and can be beaten en passant (-1: none)

// saves with which rooks, the king can still castle
bool whiteCastlingLeft = true;
bool whiteCastlingRight = true;
bool blackCastlingLeft = true;
bool blackCastlingRight = true;

// saves which pieces on the board are created using pawn promotion
bool piecesByPawnPromotion[8][8] = {false};

// saves if the pawns were promoted and you will have the option to select which piece you want
bool whitePawnPromoted = false;
bool blackPawnPromoted = false;

// textures and sprites
Texture pieceTextures[12];
Sprite pieceSprites[12];
Texture pieceShadowTextures[4];
Sprite pieceShadowSprites[4];
string pieceNames[6] = {"pawn", "rook", "knight", "bishop", "queen", "king"}; // required to load in the textures automatically

Texture boardTexture;
Sprite boardSprite;
Vector2i boardPosition = {(windowSizeX - 134 * pixelScale) / 2, (windowSizeY - 134 * pixelScale) / 2};

Texture boardShadowTexture;
Sprite boardShadowSprite;

Vector2i mousePosition = {-1, -1}; // saves, which field the mouse clicked on
Vector2i pickedUpPiece = {-1, -1}; // which piece is picked up (no piece = {-1, -1})
bool piecePickedUp = false; // is a piece picked up
bool whiteToMove = true; // is white player the next to move

// saves in which state the game currently is (0 = still running, 1 = white won, 2 = black won, 3 = draw)
int gameState = 0;

// game functions

// logic functions

// function that returns true if a piece would jump over another piece when moved from the piece position to the mouse position
bool isPieceInWay(Vector2i piecePosition, Vector2i mousePosition)
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
			// go through every field in between and check if there is a piece
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
			// go through every field in between and check if there is a piece
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
			// go through every field in between and check if there is a piece
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
			// add the step to the respectiv direction to both axis
			if (pieces[piecePosition.y + i * directionToMoveY][piecePosition.x + i * directionToMoveX] != 0)
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

// function that returns if the given piece can jump to the given position (from piece position to mouse position), without checking if the king would be in check
bool canPieceMoveHere(Vector2i piecePosition, Vector2i mousePosition)
{
	// saves the index of the current piece
	int piece = pieces[piecePosition.y][piecePosition.x] - 1;

	/*
		general rules
	*/
	// pieces can't be placed on the same color
	if (((piece >= 0 && piece <= 5) && (pieces[mousePosition.y][mousePosition.x] - 1 >= 0 && pieces[mousePosition.y][mousePosition.x] - 1 <= 5)) ||
	((piece >= 6 && piece <= 11) && (pieces[mousePosition.y][mousePosition.x] - 1 >= 6 && pieces[mousePosition.y][mousePosition.x] - 1 <= 11)))
	{
		return false;
	}

	/*
		piece rules
	*/
	// go through every piece type
	if (piece == -1) // if the field is empty, it can't move anywhere
	{
		return false;
	}
	else if (piece == 0) // white pawn
	{
		// if the piece moves 1 field forward
		if (piecePosition.y - 1 == mousePosition.y)
		{
			// check if the pawn wants to move straight if no piece is ahead
			if (piecePosition.x == mousePosition.x && pieces[piecePosition.y - 1][piecePosition.x] == 0)
			{
				return true;
			}
			// check if the pawn wants to move diagonal if there is a piece to beat
			// left
			else if (piecePosition.x - 1 == mousePosition.x && ((pieces[piecePosition.y - 1][piecePosition.x - 1] - 1 >= 6 && 
			pieces[piecePosition.y - 1][piecePosition.x - 1] - 1 <= 11) || (mousePosition.x == blackEnPassant && piecePosition.y == 3)))
			{
				return true;
			}
			// right
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
		// if the pawn wants to move two field forward
		else if (piecePosition.y - 2 == mousePosition.y)
		{
			// check if the pawn is still on the "base line" and there is not piece in the way
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
		// if the piece moves 1 field forward (globaly backwards)
		if (piecePosition.y + 1 == mousePosition.y)
		{
			// check if the pawn wants to move straight if no piece is ahead
			if (piecePosition.x == mousePosition.x && pieces[piecePosition.y + 1][piecePosition.x] == 0)
			{
				return true;
			}
			// check if the pawn wants to move diagonal if there is a piece to beat
			// left
			else if (piecePosition.x - 1 == mousePosition.x && ((pieces[piecePosition.y + 1][piecePosition.x - 1] - 1 >= 0 && 
			pieces[piecePosition.y + 1][piecePosition.x - 1] - 1 <= 5) || (mousePosition.x == whiteEnPassant && piecePosition.y == 4)))
			{
				return true;
			}
			// right
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
		// if the pawn wants to move two field forward (globaly backwards)
		else if (piecePosition.y + 2 == mousePosition.y)
		{
			// check if the pawn is still on the "base line" and there is not piece in the way
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
		// and there is no piece in the way
		else if (isPieceInWay(piecePosition, mousePosition))
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
		// and there is no piece in the way
		else if (isPieceInWay(piecePosition, mousePosition))
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
		// and there is no piece in the way
		else if (isPieceInWay(piecePosition, mousePosition))
		{
			return false;
		}
	}
	else if (piece == 5 || piece == 11) // black and white king
	{
		// check all castle positions
		if (piece == 5 && mousePosition == Vector2i{1, 7} && whiteCastlingLeft && pieces[7][1] == 0 && pieces[7][2] == 0 && pieces[7][3] == 0)
		{
			return true;
		}
		else if (piece == 5 && mousePosition == Vector2i{6, 7} && whiteCastlingRight && pieces[7][5] == 0 && pieces[7][6] == 0)
		{
			return true;
		}
		else if (piece == 11 && mousePosition == Vector2i{1, 0} && blackCastlingLeft && pieces[0][1] == 0 && pieces[0][2] == 0 && pieces[0][3] == 0)
		{
			return true;
		}
		else if (piece == 11 && mousePosition == Vector2i{6, 0} && blackCastlingRight && pieces[0][5] == 0 && pieces[0][6] == 0)
		{
			return true;
		}
		// else, make sure it only moves one field at a time
		else if (abs(piecePosition.x - mousePosition.x) > 1 || abs(piecePosition.y - mousePosition.y) > 1)
		{
			return false;
		}
	}

	// else the piece can jump to the given position, so return true
	return true;
}

// function that returns if the king is in check
bool isKingInCheck()
{
	Vector2i kingPosition;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if ((whiteToMove && pieces[y][x] - 1 == 5) || (!whiteToMove && pieces[y][x] - 1 == 11))
			{
				kingPosition = {x, y};
			}
		}
	}

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if ((whiteToMove && pieces[y][x] - 1 >= 6 && pieces[y][x] - 1 <= 11) ||
			(!whiteToMove && pieces[y][x] - 1 >= 0 && pieces[y][x] - 1 <= 5))
			{
				if (canPieceMoveHere(Vector2i(x, y), kingPosition))
				{
					return true;
				}
			}
		}
	}
	return false;
}

// function that returns if a piece can actually jump to the given position, with looking at the king and whether he would be in check or not
bool canPieceActuallyMoveHere(Vector2i piecePosition, Vector2i mousePosition)
{
	// if the piece is put back at the same spot, return true, because qou should be able to put a piece back
	if (piecePosition.x == mousePosition.x && piecePosition.y == mousePosition.y)
	{
		return true;
	}
	// if the piece is moved somewhere else
	else
	{
		// first check if the piece can even move to the position
		if (canPieceMoveHere(piecePosition, mousePosition))
		{
			// if the piece is a king and you want to castle to the left, check if the king is or would be in check in the process, because castling out of check is illegal
			if ((pieces[piecePosition.y][piecePosition.x] - 1 == 5 && mousePosition == Vector2i{1, 7} && whiteCastlingLeft) ||
			(pieces[piecePosition.y][piecePosition.x] - 1 == 11 && mousePosition == Vector2i{1, 0} && blackCastlingLeft))
			{
				// check if the king is in check
				if (isKingInCheck())
				{
					return false;
				}

				// check every position the king would move through when castling
				for (int x = 3; x >= 1; x--)
				{
					// move the king to the position
					pieces[piecePosition.y][x] = pieces[piecePosition.y][piecePosition.x];
					pieces[piecePosition.y][piecePosition.x] = 0;

					// check if he would be in check
					if (isKingInCheck())
					{
						// if yes, move him back and return false
						pieces[piecePosition.y][piecePosition.x] = pieces[piecePosition.y][x];
						pieces[piecePosition.y][x] = 0;

						return false;
					}

					// if not, move him back anyways, because the moving takes place in the updateGameFunction, this function is only there, to check if it would be possible
					pieces[piecePosition.y][piecePosition.x] = pieces[piecePosition.y][x];
					pieces[piecePosition.y][x] = 0;
				}

				return true;
			}
			// else if the piece is a king and you want to castle to the right, check if the king is or would be in check in the process, because castling out of check is illegal
			else if ((pieces[piecePosition.y][piecePosition.x] - 1 == 5 && mousePosition == Vector2i{6, 7} && whiteCastlingRight) ||
			(pieces[piecePosition.y][piecePosition.x] - 1 == 11 && mousePosition == Vector2i{6, 0} && blackCastlingRight))
			{
				// check if the king is in check
				if (isKingInCheck())
				{
					return false;
				}

				// check every position the king would move through when castling
				for (int x = 5; x <= 6; x++)
				{
					// move the king to the position
					pieces[piecePosition.y][x] = pieces[piecePosition.y][piecePosition.x];
					pieces[piecePosition.y][piecePosition.x] = 0;

					// check if he would be in check
					if (isKingInCheck())
					{
						// if yes, move him back and return false
						pieces[piecePosition.y][piecePosition.x] = pieces[piecePosition.y][x];
						pieces[piecePosition.y][x] = 0;

						return false;
					}

					// if not, move him back anyways, because the moving takes place in the updateGameFunction, this function is only there, to check if it would be possible
					pieces[piecePosition.y][piecePosition.x] = pieces[piecePosition.y][x];
					pieces[piecePosition.y][x] = 0;
				}

				return true;
			}
			// else if every other piece (including the king, just not castling) is moved, check if the own king would be in check
			else
			{
				// move the piece to the position
				int piece = pieces[mousePosition.y][mousePosition.x];
				pieces[mousePosition.y][mousePosition.x] = pieces[piecePosition.y][piecePosition.x];
				pieces[piecePosition.y][piecePosition.x] = 0;

				// check if the king would be in check
				if (isKingInCheck())
				{
					// if yes, move the piece back and return false
					pieces[piecePosition.y][piecePosition.x] = pieces[mousePosition.y][mousePosition.x];
					pieces[mousePosition.y][mousePosition.x] = piece;

					return false;
				}
				else
				{
					// if not, move the piece back anyways, because the moving takes place in the updateGameFunction, this function is only there, 
					// to check if it would be possible
					pieces[piecePosition.y][piecePosition.x] = pieces[mousePosition.y][mousePosition.x];
					pieces[mousePosition.y][mousePosition.x] = piece;
					
					return true;
				}
			}
		}
		// if the piece can't move to the position in the first place
		else
		{
			return false;
		}
	}
	return false;
}

// function that updates the list of valid positions a piece can move to
void updateValidPiecePositions(Vector2i piecePosition)
{
	// go through every field
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			// and check if the piece can move there
			validPiecePositions[y][x] = canPieceActuallyMoveHere(piecePosition, Vector2i(x, y));
		}
	}
}

// function that return if the current player is checkmate
bool isCheckmate()
{
	// check if the king is in check, if he wouldn't be in check, it would be checkmate (but maybe stalemate)
	if (isKingInCheck())
	{
		// go trough every field (to check for own pieces)
		for (int x1 = 0; x1 < 8; x1++)
		{
			for (int y1 = 0; y1 < 8; y1++)
			{
				// if the piece has the own color, check every position it could jump to and if this would save the king
				if ((whiteToMove && pieces[y1][x1] - 1 >= 0 && pieces[y1][x1] - 1 <= 5) || (!whiteToMove && pieces[y1][x1] - 1 >= 6 && pieces[y1][x1] - 1 <= 11))
				{
					// go again through every field (to check where the current piece can jump to)
					for (int x2 = 0; x2 < 8; x2++)
					{
						for (int y2 = 0; y2 < 8; y2++)
						{
							// if the piece isn't moved at all, we don't have to check, because it doesn't change anything
							if (x1 == x2 && y1 == y2)
							{
								continue;
							}
							
							// check if the piece has valid fields to move to (since canPieceActuallyMoveHere is also cheking if the king would be in check, we can use it here
							// to check if the player still has an option to save the king)
							if (canPieceActuallyMoveHere(Vector2i(x1, y1), Vector2i(x2, y2)))
							{
								// if one piece is found, we can return false, since the player still has the chance to save the king
								return false;
							}
						}
					}
				}
			}
		}
		// if no piece can save the king, the player is checkmate
		return true;
	}
	return false;
}

// function that returns if the current player can't move the king anywhere but in check -> stalemate
bool isStalemate()
{
	// check if the king isn't in check, because this is necessary for stalemate
	if (!isKingInCheck())
	{
		// if every player only has a king and either one rook or one bishop, the game ends in stalemate
		if (beatenPieces[0] == 8 && beatenPieces[1] == 2 && ((beatenPieces[2] == 2 && beatenPieces[3] == 1) || (beatenPieces[2] == 1 && beatenPieces[3] == 2) ||
		(beatenPieces[2] == 2 && beatenPieces[3] == 2)) && beatenPieces[4] == 1 
		&& beatenPieces[6] == 8 && beatenPieces[7] == 2 && ((beatenPieces[8] == 2 && beatenPieces[9] == 1) || (beatenPieces[8] == 1 && beatenPieces[9] == 2) || 
		(beatenPieces[8] == 2 && beatenPieces[9] == 2)) && beatenPieces[10] == 1)
		{
			return true;
		}

		// go through every field (to check for own pieces)
		for (int x1 = 0; x1 < 8; x1++)
		{
			for (int y1 = 0; y1 < 8; y1++)
			{
				// if the piece has the own color, check if it has any position to jump to, without putting the king in check
				if ((whiteToMove && pieces[y1][x1] - 1 >= 0 && pieces[y1][x1] - 1 <= 5) || (!whiteToMove && pieces[y1][x1] - 1 >= 6 && pieces[y1][x1] - 1 <= 11))
				{
					// go again through every field (to check where the current piece can jump to)
					for (int x2 = 0; x2 < 8; x2++)
					{
						for (int y2 = 0; y2 < 8; y2++)
						{
							// if the piece isn't moved at all, we don't have to check, because it doesn't change anything
							if (x1 == x2 && y1 == y2)
							{
								continue;
							}
							
							// check if the piece has valid fields to move to (since canPieceActuallyMoveHere is also cheking if the king would be in check, we can use it here
							// to check if the player still has an option to move to whitout putting the king in check)
							if (canPieceActuallyMoveHere(Vector2i(x1, y1), Vector2i(x2, y2)))
							{
								// if one piece is found, we can return false, since the player still has the chance to move
								return false;
							}
						}
					}
				}
			}
		}

		// if no piece can be moved without putting the king in check, the game ends in stalemate
		return true;
	}
	return false;
}

// function to update the list of beaten pieces
void updateBeatenPieces()
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

		// because, you pawn promotion will result in a different amount of start pieces, acount for that, by checking which pieces where created
		// by pawn promotion and couting them as pawns
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (pieces[y][x] - 1 == i)
				{
					if (piecesByPawnPromotion[y][x])
					{
						if (i < 6)
						{
							beatenPieces[0] -= 1;
						}
						else
						{
							beatenPieces[6] -= 1;
						}
					}
					else
					{
						beatenPieces[i] -= 1;
					}
				}
			}
		}
	}
}

// main function to update the game, pieces get moved here and all previous function get called here
void updateGame(Event event, RenderWindow &window)
{
	if (!playOnline || (playOnline && ((youAreHost && (whiteToMove || whitePawnPromoted)) || (!youAreHost && (!whiteToMove || blackPawnPromoted)))))
	{
		// if the game has ended and for some reason this function still gets called, simply return
		if (gameState != 0)
		{
			return;
		}
		
		// if the left mousebutton got pressed
		if (event.mouseButton.button == Mouse::Left)
		{
			// if a white pawn promoted, the player first has to select the piece he want's, before anyone can move
			if (whitePawnPromoted)
			{
				// go trough the positions of the four pieces, that you can choose
				for (int x = 0; x < 4; x++)
				{
					// if the player clicked on one
					if (Mouse::getPosition(window).y > boardPosition.y + 3 * pixelScale && Mouse::getPosition(window).y < boardPosition.y + 3 * pixelScale + 16 * pixelScale && 
					Mouse::getPosition(window).x > boardPosition.x + 3 * pixelScale + (mousePosition.x - 1.5 + x) * 16 * pixelScale &&
					Mouse::getPosition(window).x < boardPosition.x + 3 * pixelScale + (mousePosition.x - 0.5 + x) * 16 * pixelScale)
					{
						// add the piece to the field
						pieces[mousePosition.y][mousePosition.x] = x + 2;

						whitePawnPromoted = false;

						// send over the pieces, to update the board for the other person
						if (playOnline)
						{
							Packet packet;
							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << pieces[y][x];
								}
							}

							packet << whiteEnPassant << blackEnPassant;

							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << piecesByPawnPromotion[y][x];
								}
							}

							socket.send(packet);

							cout << "Pieces were send." << endl;
						}
					}
				}
			}
			// if a black pawn promoted, the player first has to select the piece he want's, before anyone can move
			else if (blackPawnPromoted)
			{
				// go trough the positions of the four pieces, that you can choose
				for (int x = 0; x < 4; x++)
				{
					// if the player clicked on one
					if (Mouse::getPosition(window).y > boardPosition.y + 3 * pixelScale + 7 * 16 * pixelScale && Mouse::getPosition(window).y < boardPosition.y + 3 * pixelScale + 16 * pixelScale + 8 * 16 * pixelScale && 
					Mouse::getPosition(window).x > boardPosition.x + 3 * pixelScale + (mousePosition.x - 1.5 + x) * 16 * pixelScale &&
					Mouse::getPosition(window).x < boardPosition.x + 3 * pixelScale + (mousePosition.x - 0.5 + x) * 16 * pixelScale)
					{
						// add the piece to the field
						pieces[mousePosition.y][mousePosition.x] = x + 8;
						pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;

						blackPawnPromoted = false;

						// send over the pieces, to update the board for the other person
						if (playOnline)
						{
							Packet packet;
							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << pieces[y][x];
								}
							}

							packet << whiteEnPassant << blackEnPassant;

							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << piecesByPawnPromotion[y][x];
								}
							}

							socket.send(packet);

							cout << "Pieces were send." << endl;
						}
					}
				}
			}
			// if the mouse isn't on the board
			else if (Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale < 0 || Mouse::getPosition(window).y - boardPosition.y - 3 * 
			pixelScale < 0 || Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale > 16 * 8 * pixelScale || 
			Mouse::getPosition(window).y - boardPosition.y - 3 * pixelScale > 16 * 8 * pixelScale)
			{
				mousePosition = {-1, -1};

				// put piece back, when one is picked up
				if (piecePickedUp)
				{
					pickedUpPiece = {-1, -1};
					piecePickedUp =  false;
				}
			}
			// if the mouse is on the board
			else
			{						
				// calculate mouse position on the board
				mousePosition = {(Mouse::getPosition(window).x - boardPosition.x - 3 * pixelScale) / (16 * pixelScale), 
				(Mouse::getPosition(window).y - boardPosition.y - 3 * pixelScale) / (16 * pixelScale)};

				// if no piece is picked up and at the mouse position is a piece
				if (!piecePickedUp && pieces[mousePosition.y][mousePosition.x] != 0)
				{
					// make sure the player can only pick up his pieces
					// if white is to move and the piece is white or black is to move and the piece is black, pick the piece up
					if ((whiteToMove && pieces[mousePosition.y][mousePosition.x] - 1 >= 0 && pieces[mousePosition.y][mousePosition.x] - 1 <= 5) ||
					(!whiteToMove && pieces[mousePosition.y][mousePosition.x] - 1 >= 6 && pieces[mousePosition.y][mousePosition.x] - 1 <= 11))
					{
						pickedUpPiece = mousePosition;
						piecePickedUp = true;
						// update the positions, the current picked up piece can be put
						updateValidPiecePositions(pickedUpPiece);
					}
				}
				// if you place the picked up piece on a valid field
				else if (piecePickedUp && validPiecePositions[mousePosition.y][mousePosition.x])
				{
					// if you put the piece not back at the same spot
					if (pickedUpPiece != mousePosition)
					{
						// if a white pawn is moved two fields forward, enable en passant for it
						if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 0 && pickedUpPiece.y - 2 == mousePosition.y)
						{
							whiteEnPassant = pickedUpPiece.x;
						}
						// if a black pawn is moved two fields forward (globally backwards), enable en passant for it
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 6 && pickedUpPiece.y + 2 == mousePosition.y)
						{
							blackEnPassant = pickedUpPiece.x;
						}
						// if something else is moved, remove the en passant option
						else if (whiteToMove)
						{
							whiteEnPassant = -1;
						}
						else if (!whiteToMove)
						{
							blackEnPassant = -1;
						}

						// if a piece, that was created using pawn promotion, is moved, the position in the variable "piecesByPawnPromotion" needs to be updated
						if (piecesByPawnPromotion[pickedUpPiece.y][pickedUpPiece.x])
						{
							piecesByPawnPromotion[pickedUpPiece.y][pickedUpPiece.x] = false;
							piecesByPawnPromotion[mousePosition.y][mousePosition.x] = true;
						}

						// if en passant is used from white pawn
						if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 0 && pickedUpPiece.y == 3 && mousePosition.x == blackEnPassant)
						{
							// move the pawn
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// remove the pawn beaten by en passant
							pieces[mousePosition.y + 1][mousePosition.x] = 0;
						}
						// if en passant is used from black pawn
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 6 && pickedUpPiece.y == 4 && mousePosition.x == whiteEnPassant)
						{
							// move the pawn
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// remove the pawn beaten by en passant
							pieces[mousePosition.y - 1][mousePosition.x] = 0;
						}
						// if white king castles to the left, we need to move the rook too
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 5 && mousePosition == Vector2i{1, 7} && whiteCastlingLeft)
						{
							// move the king
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// move the rook
							pieces[7][2] = pieces[7][0];
							pieces[7][0] = 0;
						}
						// if white king castles to the right, we need to move the rook too
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 5 && mousePosition == Vector2i{6, 7} && whiteCastlingRight)
						{
							// move the king
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// move the rook
							pieces[7][5] = pieces[7][7];
							pieces[7][7] = 0;
						}
						// if black king castles to the left, we need to move the rook too
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 11 && mousePosition == Vector2i{1, 0} && blackCastlingLeft)
						{
							// move the king
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// move the rook
							pieces[0][2] = pieces[0][0];
							pieces[0][0] = 0;
						}
						// if black king castles to the right, we need to move the rook too
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 11 && mousePosition == Vector2i{6, 0} && blackCastlingRight)
						{
							// move the king
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
							// move the rook
							pieces[0][5] = pieces[0][7];
							pieces[0][7] = 0;
						}
						// if a white pawn is promoted
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 0 && mousePosition.y == 0)
						{
							// move the pawn
							pieces[mousePosition.y][mousePosition.x] = 1;
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;

							// enable the option to select a piece
							piecesByPawnPromotion[mousePosition.y][mousePosition.x] = true;
							whitePawnPromoted = true;
						}
						// if a black pawn is promoted
						else if (pieces[pickedUpPiece.y][pickedUpPiece.x] - 1 == 6 && mousePosition.y == 7)
						{
							// move the pawn
							pieces[mousePosition.y][mousePosition.x] = 7;
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;

							// enable the option to select a piece
							piecesByPawnPromotion[mousePosition.y][mousePosition.x] = true;
							blackPawnPromoted = true;
						}
						// else if a piece is moved normally
						else
						{
							// switch the pieces (empty <-> pickedUpPiece)
							pieces[mousePosition.y][mousePosition.x] = pieces[pickedUpPiece.y][pickedUpPiece.x];
							pieces[pickedUpPiece.y][pickedUpPiece.x] = 0;
						}

						// check if a rook or the king is moved to determine if the king can still castle
						// if one of the white rooks moves, only disable castling with this rook
						if (pieces[0][0] - 1 != 7 && blackCastlingLeft)
						{
							blackCastlingLeft = false;
						}
						if (pieces[0][7] - 1 != 7 && blackCastlingRight)
						{
							blackCastlingRight = false;
						}
						if (pieces[7][0] - 1 != 1 && whiteCastlingLeft)
						{
							whiteCastlingLeft = false;
						}
						if (pieces[7][7] - 1 != 1 && whiteCastlingRight)
						{
							whiteCastlingRight = false;
						}

						// if white king moves, disable castling for white completly
						if (pieces[0][4] - 1 != 11 && (blackCastlingLeft || blackCastlingRight))
						{
							blackCastlingLeft = false;
							blackCastlingRight = false;
						}
						// if black king moves, disable castling for white completly
						if (pieces[7][4] - 1 != 5 && (whiteCastlingLeft || whiteCastlingRight))
						{
							whiteCastlingLeft = false;
							whiteCastlingRight = false;
						}

						// send over the pieces, to update the board for the other person
						if (playOnline && ((youAreHost && !whitePawnPromoted) || (!youAreHost && !blackPawnPromoted)))
						{
							Packet packet;
							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << pieces[y][x];
								}
							}

							packet << whiteEnPassant << blackEnPassant;

							for (int x = 0; x < 8; x++)
							{
								for (int y = 0; y < 8; y++)
								{
									packet << piecesByPawnPromotion[y][x];
								}
							}

							socket.send(packet);

							cout << "Pieces were send." << endl;
						}

						// change player turn
						whiteToMove = !whiteToMove;
						
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
					}

					// put the piece down
					pickedUpPiece = {-1, -1};
					piecePickedUp = false;
				}
			}
		}
		// if right clicked, put the piece back
		else if (event.mouseButton.button == Mouse::Right) // when right mouse button is pressed, put piece back
		{
			if (piecePickedUp)
			{
				pickedUpPiece = {-1, -1};
				piecePickedUp =  false;
			}
		}
	}
}

// function to reset the game, so you can start over
void resetGame()
{
	// reset the board
	int startPieces[8][8] = {
    {8, 9, 10, 11, 12, 10, 9, 8},
    {7, 7, 7, 7, 7, 7, 7, 7},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 3, 4, 5, 6, 4, 3, 2}
	};
	memcpy(pieces, startPieces, sizeof(pieces));

	// reset all other variables
	int startBeatenPieces[12] = {0};
	memcpy(beatenPieces, startBeatenPieces, sizeof(beatenPieces));

	whiteEnPassant = -1;
	blackEnPassant = -1;

	whiteCastlingLeft = true;
	whiteCastlingRight = true;
	blackCastlingLeft = true;
	blackCastlingRight = true;

	bool startPiecesByPawnPromotion[8][8] = {false};
	memcpy(piecesByPawnPromotion, startPiecesByPawnPromotion, sizeof(piecesByPawnPromotion));

	whitePawnPromoted = false;
	blackPawnPromoted = false;

	mousePosition = {-1, -1};
	pickedUpPiece = {-1, -1};
	piecePickedUp = false;
	whiteToMove = true;

	gameState = 0;
}


// drawing functions

// function to load all textures for the pieces
int loadGamePieceTextures()
{
	// load textures of white pieces
	for (int i = 0; i < 6; i++)
	{
		if (!pieceTextures[i].loadFromFile("textures\\" + pieceNames[i] + "_white.png"))
		{
			cout << "Couldn't load texture \"textures\\" + pieceNames[i] + "_white.png\". Exiting.." << endl;
			return 1;
		}

		pieceSprites[i].setTexture(pieceTextures[i]);
		pieceSprites[i].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}
	
	// load texture of black pieces
	for (int i = 0; i < 6; i++)
	{
		if (!pieceTextures[i + 6].loadFromFile("textures\\" + pieceNames[i] + "_black.png"))
		{
			cout << "Couldn't load texture \"textures\\" + pieceNames[i] + "_black.png\". Exiting.." << endl;
			return 1;
		}

		pieceSprites[i + 6].setTexture(pieceTextures[i + 6]);
		pieceSprites[i + 6].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}

	for (int i = 0; i < 4; i++)
	{
		if (!pieceShadowTextures[i].loadFromFile("textures\\" + to_string(i + 1) + "_piece_shadow.png"))
		{
			cout << "Couldn't load texture \"textures\\" + to_string(i + 1) + "_piece_shadow.png\". Exiting.." << endl;
			return 1;
		}

		pieceShadowSprites[i].setTexture(pieceShadowTextures[i]);
		pieceShadowSprites[i].setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
	}
	
	return 0;
}

// function to load all textures
void loadGameTextures()
{
    // load piece textures
    loadGamePieceTextures();

    // load other textures
    // load the board texture
    if (!boardTexture.loadFromFile("textures\\board.png"))
    {
        cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
        return;
    }

    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(pixelScale, pixelScale); // one pixel on the images is equal to five pixel on the window
    boardSprite.setPosition(boardPosition.x, boardPosition.y);

    // load the texture of the boards shadow
    if (!boardShadowTexture.loadFromFile("textures\\board_shadow.png"))
    {
        cout << "Couldn't load texture \"textures\\board.png\". Exiting.." << endl;
        return;
    }

    boardShadowSprite.setTexture(boardShadowTexture);
    boardShadowSprite.setScale(pixelScale, pixelScale);
    boardShadowSprite.setPosition(boardPosition.x + 3 * pixelScale, boardPosition.y + 3 * pixelScale); // shift 3 pixels on the image scale further
}

// function to draw one piece at a position
void drawPiece(RenderWindow &window, int piece, Vector2i position)
{
	// set position for the piece
	pieceSprites[piece].setPosition(position.x, position.y);

	// adjust some piece positions, so that all line up (because some pieces are taller)
	if (piece == 1 || piece == 7) // rook = one pixel down
	{
		Vector2 position = pieceSprites[piece].getPosition();
		position.y -= 1 * pixelScale;
		pieceSprites[piece].setPosition(position);
	}
	else if (piece == 2 || piece == 8) // knight = two pixels down
	{
		Vector2 position = pieceSprites[piece].getPosition();
		position.y -= 2 * pixelScale;
		pieceSprites[piece].setPosition(position);
	}
	else if (piece != 0 && piece != 6) // bishop, queen and king = 3 pixels down
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

	// set the shadows position (two pixels to the right and 8 pixels down)
	pieceShadowSprites[currentShadowIndex].setPosition(position.x + 2 * pixelScale, position.y + 8 * pixelScale);

	// draw piece and shadow
	window.draw(pieceSprites[piece]);
	window.draw(pieceShadowSprites[currentShadowIndex], BlendMultiply); // change blendmode to make it transparent
}

// function to draw all pieces
void drawPieces(RenderWindow &window)
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
				drawPiece(window, pieces[y][x] - 1, position);
			}
			// draw other pieces on the board
			else
			{
				Vector2i position = {boardPosition.x + 7 * pixelScale + x * 16 * pixelScale, boardPosition.y + 3 * pixelScale + y * 16 * pixelScale};
				drawPiece(window, pieces[y][x] - 1, position);
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
			drawPiece(window, i, position);
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
			drawPiece(window, i, position);
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

// function to draw invalid fields (fields, that the currently picked up piece can't jump to)
void drawInavlidFields(RenderWindow &window)
{
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
}

// function to draw option to select, when pawn is promoted
void drawPawnPromotionOptions(RenderWindow &window)
{
	// if a pawn was promoted
    if (whitePawnPromoted || blackPawnPromoted)
    {
		// darken the window
        RectangleShape windowDarken;
        windowDarken.setFillColor(Color(120, 120, 120));
        windowDarken.setSize(Vector2f(windowSizeX, windowSizeY));

        window.draw(windowDarken, BlendMultiply);

		// draw the options
        if (whitePawnPromoted)
        {
			// go through every possible option (rook, knight, bishop and queen) and draw them side by side
            for (int i = 0; i < 4; i++)
            {
				// draw the piece
                Vector2i position = {boardPosition.x + 7 * pixelScale + (mousePosition.x - 1.5 + i) * 16 * pixelScale, boardPosition.y + 3 * pixelScale};

                drawPiece(window, i + 1, position);
            }
        }
        else if (blackPawnPromoted)
        {
			// go through every possible option (rook, knight, bishop and queen) and draw them side by side
            for (int i = 0; i < 4; i++)
            {
				// draw the piece
                Vector2i position = {boardPosition.x + 7 * pixelScale + (mousePosition.x - 1.5 + i) * 16 * pixelScale, boardPosition.y + 3 * pixelScale + 7 * 16 * pixelScale};

                drawPiece(window, i + 7, position);
            }
        }
    }
}

// function to draw the game
void drawGame(RenderWindow &window)
{
    // draw board
    window.draw(boardShadowSprite, BlendMultiply);
    window.draw(boardSprite);

	// draw invalid fields, when piece is picked up
    drawInavlidFields(window);

	// draw pieces (including beaten pieces)
    drawPieces(window);

	// draw the pieces you can select if a pawn was promoted
    drawPawnPromotionOptions(window);
}

#endif