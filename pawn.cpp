#include "pawn.h"
#include "textureManager.h"
#include <iostream>



Pawn::Pawn(const char* filename, SDL_Renderer* ren, int bRow, int bColumn, char c)
	: Piece(filename, ren, bRow, bColumn, c), color(EMPTY) 
{
    currentColPreviousPawn = currentRowPreviousPawn = previousRowPreviousPawn = -1;
}

Pawn::~Pawn() {}

static int previousRow, previousCol;

bool Pawn::move(int toRow, int toCol, char blueRectangles[][8])
{
	return blueRectangles[toRow][toCol] == BLUE_RECTANGLE;
}

void Pawn::displayBlueRectangles(int fromRow, int fromCol, int board[][8], char colors[][8], char boardToUpdate[][8], bool forChecks)
{
    if (colors[fromRow][fromCol] == WHITE)
    {
        // White pawn moves
        if (fromRow + 2 < 8 && colors[fromRow + 2][fromCol] == EMPTY && fromRow == 1)
            boardToUpdate[fromRow + 2][fromCol] = BLUE_RECTANGLE;
        if (fromRow + 1 < 8 && colors[fromRow + 1][fromCol] == EMPTY)
            boardToUpdate[fromRow + 1][fromCol] = BLUE_RECTANGLE;
        if (fromRow + 1 < 8 && fromCol + 1 < 8 && colors[fromRow + 1][fromCol + 1] != EMPTY && colors[fromRow + 1][fromCol + 1] == BLACK)
            boardToUpdate[fromRow + 1][fromCol + 1] = BLUE_RECTANGLE;
        if (fromRow + 1 < 8 && fromCol - 1 >= 0 && colors[fromRow + 1][fromCol - 1] != EMPTY && colors[fromRow + 1][fromCol - 1] == BLACK)
            boardToUpdate[fromRow + 1][fromCol - 1] = BLUE_RECTANGLE;

        // check for "bicie w przelocie" for white pawns
        if (previousRowPreviousPawn - currentRowPreviousPawn == 2)
        {
            // From the left site
            if (currentColPreviousPawn + 1 == fromCol && colors[fromRow + 1][fromCol - 1] == EMPTY && fromCol + 1 < 8 && fromRow + 1 < 8)
                boardToUpdate[fromRow + 1][fromCol - 1] = BLUE_RECTANGLE;
            // From the right site
            if (currentColPreviousPawn - 1 == fromCol && colors[fromRow + 1][fromCol + 1] == EMPTY && fromCol + 1 < 8 && fromRow + 1 < 8)
                boardToUpdate[fromRow + 1][fromCol + 1] = BLUE_RECTANGLE;
        }
    }

    else if (colors[fromRow][fromCol] == BLACK)
    {
        // Black pawn moves
        if (fromRow - 2 >= 0 && colors[fromRow - 2][fromCol] == EMPTY && fromRow == 6)
            boardToUpdate[fromRow - 2][fromCol] = BLUE_RECTANGLE;
        if (fromRow - 1 >= 0 && colors[fromRow - 1][fromCol] == EMPTY)
            boardToUpdate[fromRow - 1][fromCol] = BLUE_RECTANGLE;
        if (fromRow - 1 >= 0 && fromCol + 1 < 8 && colors[fromRow - 1][fromCol + 1] != EMPTY && colors[fromRow - 1][fromCol + 1] == WHITE)
            boardToUpdate[fromRow - 1][fromCol + 1] = BLUE_RECTANGLE;
        if (fromRow - 1 >= 0 && fromCol - 1 >= 0 && colors[fromRow - 1][fromCol - 1] != EMPTY && colors[fromRow - 1][fromCol - 1] == WHITE)
            boardToUpdate[fromRow - 1][fromCol - 1] = BLUE_RECTANGLE;

        // check for "bicie w przelocie" for black pawns
        if (currentRowPreviousPawn - previousRowPreviousPawn == 2)
        {
            // From the left site
            if (currentColPreviousPawn + 1 == fromCol && colors[fromRow - 1][fromCol - 1] == EMPTY && fromCol - 1 >= 0 && fromRow - 1 >= 0)
                boardToUpdate[fromRow - 1][fromCol - 1] = BLUE_RECTANGLE;
            // From the right site
            if (currentColPreviousPawn - 1 == fromCol && colors[fromRow - 1][fromCol + 1] == EMPTY && fromCol - 1 >= 0 && fromCol + 1 < 8 && fromRow - 1 >= 0)
                boardToUpdate[fromRow - 1][fromCol + 1] = BLUE_RECTANGLE;
        }
    }
}

void Pawn::enPassantMove(int fromRow, int fromCol, int toRow, int toCol, char colors[][8], int board[][8], char blueRectangles[][8])
{
    if (colors[fromRow][fromCol] == WHITE)
    {
        if (blueRectangles[toRow][toCol] == BLUE_RECTANGLE && colors[toRow][toCol] == EMPTY && toRow == fromRow + 1 && toCol != fromCol)
        {
            // Remove black pawn after en passant move
            board[toRow - 1][toCol] = NONE;
            colors[toRow - 1][toCol] = EMPTY;
        }
    }
    if (colors[fromRow][fromCol] == BLACK)
    {
        if (blueRectangles[toRow][toCol] == BLUE_RECTANGLE && colors[toRow][toCol] == EMPTY && toRow == fromRow - 1 && toCol != fromCol)
        {
            // Remove black pawn after en passant move
            board[toRow + 1][toCol] = NONE;
            colors[toRow + 1][toCol] = EMPTY;
        }
    }
   
}