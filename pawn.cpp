#include "pawn.h"
#include "textureManager.h"

Pawn::Pawn(const char* filename, SDL_Renderer* ren, int x, int y, int bSquare, char c)
	: Piece(filename, ren, x, y, bSquare, c)
{}

Pawn::~Pawn() {}

void Pawn::move()
{

}
