#include "piece.h"
#include <cctype>
#include <stdexcept>

Piece::Piece(char color_, char type_) : color(color_), type(type_)
{
    // Raise error if color is not 'W' or 'B'
    if (color != 'W' && color != 'B')
    {
        throw std::invalid_argument("Color must be 'W' for white or 'B' for black.");
    }
    // Raise error if type is not one of the valid piece types
    if (type != 'P' && type != 'R' && type != 'N' && type != 'B' && type != 'Q' && type != 'K')
    {
        throw std::invalid_argument("Type must be 'P', 'R', 'N', 'B', 'Q', or 'K'.");
    }
}

// Returns W for white and B for black
char Piece::getColor() const
{
    return color;
}

// Just returns the type of the piece
char Piece::getType() const
{
    return type;
}

char Piece::getSymbol() const
{
    if (color == 'W')
        return type; // Uppercase for white pieces
    else
        return std::tolower(type); // Lowercase for black pieces
}

// checks if the piece is white
bool Piece::isWhite() const
{
    return color == 'W';
}

// checks if the piece is black
bool Piece::isBlack() const
{
    return color == 'B';
}