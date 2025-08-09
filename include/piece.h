#ifndef PIECE_H
#define PIECE_H

class Piece
{
private:
    char color; // 'W' for white, 'B' for black
    char type;  // 'P' for pawn, 'R' for rook, 'N' for knight, 'B' for bishop, 'Q' for queen, 'K' for king

public:
    Piece(char color, char type);

    char getColor() const;
    char getType() const;
    char getSymbol() const;

    bool isWhite() const;
    bool isBlack() const;
};

#endif // PIECE_H