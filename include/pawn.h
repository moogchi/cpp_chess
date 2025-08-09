#ifndef PAWN_H
#define PAWN_H

#include <vector>

#include "../piece.h"
#include "../board.h"
#include "../position.h"
#include "../color.h"

class Pawn : public Piece
{
private:
    bool canMoveForward(const Position &from, const Position &to) const;
    bool canCapture(const Position &from, const Position &to) const;
    bool isEnPassant(const Position &from, const Position &to) const;
    bool isPromotion(const Position &from, const Position &to) const;

public:
    Pawn(Color color);
    char getColor() const;
    char getSymbol() const;
    std::vector<Position> getValidMoves(const Position &from, const Board &board) const;
};

#endif // PAWN_H