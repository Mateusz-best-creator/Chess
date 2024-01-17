#include "board.h"
#include <typeinfo>

// Helper function
void Board::resetBlueRectanglesBoard()
{
	// Reset blue rectangles
	for (size_t i = 0; i < HEIGHT; i++)
		for (size_t j = 0; j < WIDTH; j++)
			blueRectanglesBoard[i][j] = EMPTY;
}

void Board::movingPiece(int row, int column, int& playerIndex)
{
	// Subtract one beacuse arrays are indexes from 0 (rows and cols from 1)
	row--;
	column--;
	if (board[row][column] == NONE && (toRow != INITIAL_VALUE && toCol != INITIAL_VALUE))
		return;

	if (fromRow == INITIAL_VALUE && fromCol == INITIAL_VALUE)
	{
		movingPieceType = board[row][column];
		if (movingPieceType == NONE)
			return;
		fromRow = row;
		fromCol = column;
		switch (board[fromRow][fromCol])
		{
		case PAWN:
			pawn->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
			break;
		case ROOK:
			rook->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
			break;
		case KNIGHT:
			knight->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
			break;
		case BISHOP:
			bishop->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
			break;
		case QUEEN:
			queen->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false);
			break;
		case KING:
			whiteKing->displayBlueRectangles(fromRow, fromCol, board, colors, blueRectanglesBoard, false, whiteSquaresBoard, blackSquaresBoard);
		}
		return;
	}
	else
	{
		toRow = row;
		toCol = column;
	}

	// Check if index is valid
	if (fromRow < 0 || toRow < 0 || fromRow > 7 || toCol > 7)
		throw InvalidIndexException(fromRow, fromCol, toRow, toCol);

	// Check if it is player's turn
	if ((colors[fromRow][fromCol] == WHITE && playerIndex == 2) || (colors[fromRow][fromCol] == BLACK && playerIndex == 1))
	{
		fromRow = fromCol = toRow = toCol = INITIAL_VALUE;
		resetBlueRectanglesBoard();
		return;
	}
	// Update all the pieces
	bool updated = updatePieces();
	// Reset blue rectangles board
	resetBlueRectanglesBoard();

	if (updated)
	{
		playerIndex = (playerIndex == 1) ? 2 : 1;
		updateColorsSquares();
	}
	// Reset variables
	fromRow = fromCol = toRow = toCol = INITIAL_VALUE;
}

bool Board::updatePieces()
{
	switch (movingPieceType)
	{
	case PAWN:
		if (!updatePiece(pawn))
			return false;
		break;
	case ROOK:
		if (!updatePiece(rook))
			return false;
		break;
	case KNIGHT:
		if (!updatePiece(knight))
			return false;
		break;
	case BISHOP:
		if (!updatePiece(bishop))
			return false;
		break;
	case QUEEN:
		if (!updatePiece(queen))
			return false;
		break;
	case KING:
		if (colors[fromRow][fromCol] == WHITE)
		{
			// Short castle for white king
			if (!whiteKing->getHasMoved() && toCol == 6 && blueRectanglesBoard[0][6] == BLUE_RECTANGLE)
			{
				board[0][7] = NONE;
				board[0][5] = ROOK;
				colors[0][7] = EMPTY;
				colors[0][5] = WHITE;
				board[0][6] = KING;
				colors[0][6] = WHITE;
				whiteKing->getKingRow() = 0;
				whiteKing->getKingColumn() = 6;
				whiteKing->getHasMoved() = true;
				break;
			}

			if (!updatePiece(whiteKing))
				return false;
				
		}
		else if (colors[fromRow][fromCol] == BLACK)
		{
			// Short castle for black king
			if (!blackKing->getHasMoved() && toCol == 6 && blueRectanglesBoard[7][6] == BLUE_RECTANGLE)
			{
				board[7][7] = NONE;
				board[7][5] = ROOK;
				colors[7][7] = EMPTY;
				colors[7][5] = BLACK;
				board[7][6] = KING;
				colors[7][6] = BLACK;
				blackKing->getKingRow() = 0;
				blackKing->getKingColumn() = 6;
				blackKing->getHasMoved() = true;
				break;
			}
			if (!updatePiece(blackKing))
				return false;
		}
		break;
	default:
		break;
	}

	if (colors[fromRow][fromCol] == WHITE)
	{
		colors[fromRow][fromCol] = EMPTY;
		colors[toRow][toCol] = WHITE;
	}

	else if (colors[fromRow][fromCol] == BLACK)
	{
		colors[fromRow][fromCol] = EMPTY;
		colors[toRow][toCol] = BLACK;
	}

	updateColorsSquares();
	if ((whiteSquaresBoard[blackKing->getKingRow()][blackKing->getKingColumn()] != EMPTY && colors[toRow][toCol] == BLACK)
		|| (blackSquaresBoard[whiteKing->getKingRow()][whiteKing->getKingColumn()] != EMPTY && colors[toRow][toCol] == WHITE))
	{
		std::cout << "Move is invalid\n";
		board[toRow][toCol] = EMPTY;
		switch (movingPieceType)
		{
		case PAWN:
			board[fromRow][fromCol] = PAWN;
			break;
		case KNIGHT:
			board[fromRow][fromCol] = KNIGHT;
			break;
		case BISHOP:
			board[fromRow][fromCol] = BISHOP;
			break;
		case ROOK:
			board[fromRow][fromCol] = ROOK;
			break;
		case QUEEN:
			board[fromRow][fromCol] = QUEEN;
			break;
		}

		if (colors[toRow][toCol] == WHITE)
		{
			colors[fromRow][fromCol] = WHITE;
			colors[toRow][toCol] = EMPTY;
		}
		else if (colors[toRow][toCol] == BLACK)
		{
			colors[fromRow][fromCol] = BLACK;
			colors[toRow][toCol] = EMPTY;
		}
		updateColorsSquares();
		return false;
	}
	return true;
}

bool Board::updatePiece(std::shared_ptr<Piece> piece)
{
	// Check if move is valid
	if (!piece->move(toRow, toCol, blueRectanglesBoard))
	{
		std::cout << "You cant move from " << fromRow << ", " << fromCol << " to " << toRow << ", " << toCol << " with "
			<< typeid(*piece).name() << std::endl;
		fromRow = fromCol = toRow = toCol = INITIAL_VALUE;
		return false;
	}

	board[fromRow][fromCol] = NONE;
	if (typeid(Pawn) == typeid(*piece))
		board[toRow][toCol] = PAWN;
	else if (typeid(Rook) == typeid(*piece))
		board[toRow][toCol] = ROOK;
	else if (typeid(Knight) == typeid(*piece))
		board[toRow][toCol] = KNIGHT;
	else if (typeid(Bishop) == typeid(*piece))
		board[toRow][toCol] = BISHOP;
	else if (typeid(Queen) == typeid(*piece))
		board[toRow][toCol] = QUEEN;
	else if (typeid(King) == typeid(*piece))
		board[toRow][toCol] = KING;

	return true;
}