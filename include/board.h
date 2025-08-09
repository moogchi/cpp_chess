#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <string>

#include "piece.h"
#include "position.h"
#include "color.h"

class Board
{
private:
    std::unique_ptr<Piece> grid[8][8];
    bool is_white_turn;

public:
    Board();

    // Initialize the board with the starting position
    void initializeBoard();

    // Get the piece at a specific position
    const Piece *Board::getPieceAt(int row, int col) const;

    // Check which player's turn it is and change it
    bool isWhiteTurn() const;
    void switchTurn();

    // Check if the Position is on the board
    bool isValidPosition(int row, int col) const;

    // Return the current state of the board as a string
    std::string getBoardState() const;

    // Print the current state of the board to the console
    void printBoard() const;
};

#endif // BOARD_H