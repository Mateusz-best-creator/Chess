#include "board.h"
#include <fstream>

void Board::checkIfCheckmate(int playerIndex)
{
	std::ofstream file("result.txt", std::ios_base::out | std::ios_base::trunc);
	if (!file.is_open())
	{
		std::cerr << "Error opening result.txt file\n";
		exit(EXIT_FAILURE);
	}
	// Check for checkmate
	if (playerIndex == 1 && whiteKing->checkIfKingHasSquare(board, colors, whiteSquaresBoard, blackSquaresBoard))
	{
		if (checkIfCanPreventCheckmate(whiteKing))
			return;
		gameRunning = false;
		std::cout << "Checkmate, black pieces won!\n";
		file << "black";
	}
	else if (playerIndex == 2 && blackKing->checkIfKingHasSquare(board, colors, whiteSquaresBoard, blackSquaresBoard))
	{
		if (checkIfCanPreventCheckmate(blackKing))
			return;
		gameRunning = false;
		std::cout << "Checkmate, white pieces won!\n";
		file << "white";
	}
	file.clear();
	file.close();
}

bool Board::checkIfCanPreventCheckmate(std::shared_ptr<King>king)
{
	char kingColor = king->getKingcolor();
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (colors[i][j] == kingColor)
			{
				switch (board[i][j])
				{
				case PAWN:
					if (preventCheckmateWithPiece(pawn, PAWN, kingColor, i, j))
						return true;
					break;
				case ROOK:
					if (preventCheckmateWithPiece(rook, ROOK, kingColor, i, j))
						return true;
					break;
				case KNIGHT:
					if (preventCheckmateWithPiece(knight, KNIGHT, kingColor, i, j))
						return true;
					break;
				case BISHOP:
					if (preventCheckmateWithPiece(bishop, BISHOP, kingColor, i, j))
						return true;
					break;
				case QUEEN:
					if (preventCheckmateWithPiece(queen, QUEEN, kingColor, i, j))
						return true;
					break;
				}
			}
			resetBlueRectanglesBoard();
		}
	}
	return false;
}

bool Board::preventCheckmateWithPiece(std::shared_ptr<Piece> piece, int pieceType, char color, int i, int j)
{
	fromRow = i;
	fromCol = j;
	char enemyColor = (color == WHITE) ? BLACK : WHITE;
	piece->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
	for (int k = 0; k < HEIGHT; k++)
	{
		for (int h = 0; h < WIDTH; h++)
		{
			if (blueRectanglesBoard[k][h] == 'r')
			{
				toRow = k;
				toCol = h;
				updateBoardColors(NONE, pieceType, EMPTY, color);
				updateColorsSquares();
				if (!((whiteSquaresBoard[blackKing->getKingRow()][blackKing->getKingColumn()] != EMPTY && colors[toRow][toCol] == color)
					|| (blackSquaresBoard[whiteKing->getKingRow()][whiteKing->getKingColumn()] != EMPTY && colors[toRow][toCol] == color)))
				{
					updateBoardColors(pieceType, NONE, color, EMPTY);
					resetBlueRectanglesBoard();
					if (color == WHITE)
						whiteKing->getCheck() = true;
					else if (color == BLACK)
						blackKing->getCheck() = true;
					return true;
				}
				updateBoardColors(pieceType, NONE, color, EMPTY);
				updateColorsSquares();
			}
		}
	}
	return false;
}

void Board::updateBoardColors(int first, int second, char third, char fourth)
{
	board[fromRow][fromCol] = first;
	board[toRow][toCol] = second;
	colors[fromRow][fromCol] = third;
	colors[toRow][toCol] = fourth;
}