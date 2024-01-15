#pragma once

#include "piece.h"

class Queen : public Rook, public Bishop
{
public:
	Queen(const char* filename, SDL_Renderer* ren, int bRow, int bCol, char c);
	~Queen();

	virtual bool move(int toRow, int toCol, char blueRectangles[][8]) override;
	virtual void displayBlueRectangles(int fromRow, int fromCol, int board[][8], char colors[][8], char blueRectangles[][8]) override;
	void updateRectangles(char color, int fromRow, int fromCol, int board[][8], char colors[][8], char blueRectangles[][8]);
};