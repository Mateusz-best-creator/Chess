#pragma once

#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "king.h"
#include "queen.h"
#include "rectangle.h"
#include "errorsObjects.h"

#include <SDL_mixer.h>
#include <vector>
#include <memory>
#include <iostream>
#include <string>


const int HEIGHT = 8, WIDTH = 8, INITIAL_PIECES_NUMBER = 32;
const int PIECES_X_DISTANCE = 90, PIECES_Y_DISTANCE = 67;
const int X_OFFSET = 42, Y_OFFSET = 29;
const int PROMOTION_RECTANGLE_HEIGHT = 70, PROMOTION_RECTANGLE_WIDTH = 355, PROMOTION_RECTANGLE_X_START = 43, PROMOTION_RECTANGLE_X_END = 398;
const double PROMOTION_REACTANGLE_X_OFFSET = 88.75;

enum Colors : char { WHITE = 'w', BLACK = 'b', EMPTY = 'e', BLUE_RECTANGLE = 'r' };
enum PieceTypes : int { NONE = 0, ROOK, KNIGHT, BISHOP, KING, QUEEN, PAWN = 6 };

class Board
{

private:
	int board[HEIGHT][WIDTH] =
	{
		{1, 2, 3, 5, 4, 3, 2, 1},
		{6, 6, 6, 6, 6, 6, 6, 6},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{6, 6, 6, 6, 6, 6, 6, 6},
		{1, 2, 3, 5, 4, 3, 2, 1}
	};
	char colors[HEIGHT][WIDTH] =
	{
		{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
		{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
		{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'},
		{'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'},
		{'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}
	};
	char blueRectanglesBoard[HEIGHT][WIDTH] =
	{
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'}
	};
	// Board which indicate which squares "can be" under control for each color (useful for checks)
	char whiteSquaresBoard[HEIGHT][WIDTH] =
	{
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'},
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'},
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'}
	};
	char blackSquaresBoard[HEIGHT][WIDTH] =
	{
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'e', 'e', 'e', 'e', 'e', 'e' , 'e', 'e'},
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'},
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'},
		{'r', 'r', 'r', 'r', 'r', 'r' , 'r', 'r'}
	};

	SDL_Texture* boardTexture;
	SDL_Renderer* renderer;
	SDL_Rect sourceRect, destinationRect;

	std::vector<std::unique_ptr<Rectangle>> rectangles;
	std::vector<std::shared_ptr<Piece>> pieces; // Polymorphism
	std::shared_ptr<Pawn> pawn;
	std::shared_ptr<Rook> rook;
	std::shared_ptr<Knight> knight;
	std::shared_ptr<Bishop> bishop;
	std::shared_ptr<Queen> queen;
	std::shared_ptr<King> whiteKing;
	std::shared_ptr<King> blackKing;
	// Exact same thing as blue rectangle but with different color
	std::unique_ptr<Rectangle> redRectangle;

	int fromRow, fromCol, toRow, toCol, movingPieceType;
	enum { INITIAL_VALUE = -1 };
	bool gameRunning;

	// Sound effects
	Mix_Chunk* pieceMoveSound = Mix_LoadWAV("Sounds/piece_move_sound.wav");
	Mix_Chunk* pieceBeatSound = Mix_LoadWAV("Sounds/piece_beat_sound.wav");

	// Get image textures for promotion rectangle
	SDL_Texture* whiteImagesPromotionTextures[4];
	SDL_Texture* blackImagesPromotionTextures[4];

public:
	Board(const char* filename, SDL_Renderer* ren);
	~Board();

	void render();
	bool update();
	void addPieces(int i, int j, int boardR);
	void movingPiece(int row, int column, int& playerIndex);

	// Functions for updating pieces
	bool updatePieces();
	bool updatePiece(std::shared_ptr<Piece> piece);
	bool checkMoveValidation();
	void checkIfCheckmate(int);
	bool checkIfCanPreventCheckmate(std::shared_ptr<King>king);
	bool preventCheckmateWithPiece(std::shared_ptr<Piece> piece, int pieceType, char color, int i, int j);
	void updateBoardColors(int first, int second, char third, char fourth);
	void displayPromotionRentangle(int, SDL_Texture* images[4]);
	PieceTypes handlePromotionRectangle(char);

	// Functions for updating occupied squares
	void updateColorsSquares();
	void resetBlueRectanglesBoard();
};