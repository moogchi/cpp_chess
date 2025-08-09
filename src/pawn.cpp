#include "pawn.h"
#include "../color.h"
#include "../board.h"
#include "../position.h"
#include "../piece.h"

Pawn::Pawn(Color color) : Piece(color, 'P') {}

char Pawn::getSymbol() const
{
    return Piece::getSymbol();
}


std::vector<Position> Pawn::getValidMoves(const Position &from, const Board &board)
{
    int direction = (getColor() == 'W') ? -1 : 1; // White moves up, Black moves down
    int startRow = (getColor() == 'W') ? 6 : 1; // Starting row for pawns
    int promoteRow = (getColor() == 'W') ? 0 : 7; // Promotion row for pawns
}