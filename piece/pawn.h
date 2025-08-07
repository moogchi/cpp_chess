#ifndef PAWN_H
#define PAWN_H

#include "../piece.h"
#include "../board.h"
#include "position.h"
#include "color.h"

class Pawn : public Piece
{
public:
    Pawn(Color color, Position position);

    bool canMoveTo(Position position, const Board &board) const override;
    bool canMoveTwo(Position position, const Board &board) const override;
    bool canCapture(Position position, const Board &board) const override;
    bool canPromote(Position position, const Board &board) const override;
    bool canEnPassant(Position position, const Board &board) const override;
};

#endif // PAWN_H