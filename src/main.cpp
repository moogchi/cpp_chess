#include "piece.h"
#include <iostream>

int main()
{
    Piece whitePawn('W', 'P');
    Piece blackQueen('B', 'Q');

    std::cout << "White Pawn: " << whitePawn.getSymbol() << ", Color: " << whitePawn.getColor() << ", Type: " << whitePawn.getType() << "\n";
    std::cout << "Black Queen: " << blackQueen.getSymbol() << ", Color: " << blackQueen.getColor() << ", Type: " << blackQueen.getType() << "\n";

    return 0;
}