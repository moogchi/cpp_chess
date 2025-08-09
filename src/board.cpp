#include "board.h"
#include <iostream>

Board::Board() : is_white_turn(true)
{
    initializeBoard();
}

void Board::initializeBoard()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            grid[row][col] = nullptr; // Empty square
        }
    }
    // Initialize pawns
    for (int col = 0; col < 8; ++col)
    {
        grid[1][col] = std::make_unique<Pawn>('B', Position(1, col)); // Black pawns
        grid[6][col] = std::make_unique<Pawn>('W', Position(6, col)); // White pawns
    }

    // Initialize White pieces
    grid[0][0] = std::make_unique<Rook>('W', Position(0, 0));
    grid[0][1] = std::make_unique<Knight>('W', Position(0, 1));
    grid[0][2] = std::make_unique<Bishop>('W', Position(0, 2));
    grid[0][3] = std::make_unique<Queen>('W', Position(0, 3));
    grid[0][4] = std::make_unique<King>('W', Position(0, 4));
    grid[0][5] = std::make_unique<Bishop>('W', Position(0, 5));
    grid[0][6] = std::make_unique<Knight>('W', Position(0, 6));
    grid[0][7] = std::make_unique<Rook>('W', Position(0, 7));

    // Initialize Black pieces
    grid[7][0] = std::make_unique<Rook>('B', Position(7, 0));
    grid[7][1] = std::make_unique<Knight>('B', Position(7, 1));
    grid[7][2] = std::make_unique<Bishop>('B', Position(7, 2));
    grid[7][3] = std::make_unique<Queen>('B', Position(7, 3));
    grid[7][4] = std::make_unique<King>('B', Position(7, 4));
    grid[7][5] = std::make_unique<Bishop>('B', Position(7, 5));
    grid[7][6] = std::make_unique<Knight>('B', Position(7, 6));
    grid[7][7] = std::make_unique<Rook>('B', Position(7, 7));
}

bool Board::isValidPosition(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        return false; // Out of bounds
    }
    return true; // Valid position
}

const Piece *Board::getPieceAt(int row, int col) const
{
    if (isValidPosition(row, col))
    {
        return grid[row][col].get(); // Return the piece at the position
    }
    return nullptr; // Invalid position, return nullptr
}

bool Board::isWhiteTurn() const
{
    return is_white_turn;
}

void Board::switchTurn()
{
    is_white_turn = !is_white_turn;
}

std::string Board::getBoardState() const
{
    std::string place;
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (grid[r][c] != nullptr)
            {
                place += grid[r][c]->getSymbol();
            }
            else
            {
                place += '.';
            }
        }
        place += '\n';
    }
}

void Board::printBoard() const
{
    std::cout << getBoardState() << "\n";
}