#pragma once

#include "piece.h"

class Rook : public Piece
{
public:
	Rook(const char* filename, SDL_Renderer* ren, int bRow, int bCol, char c);
	~Rook();

	virtual bool move(int fromRow, int fromCol, int toRow, int toCol, int board[][8], char colors[][8], char blueRectangles[][8]);
	virtual void displayBlueRectangles(int fromRow, int fromCol, int board[][8], char colors[][8], char blueRectangles[][8]);

};