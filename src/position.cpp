#include "position.h"

Position::Position(int r, int c) : row(row), col(col) {}

// Getters for row and column
int Position::getRow() const
{
    return row;
}

int Position::getCol() const
{
    return col;
}

// Setters for row and column
void Position::setRow(int r)
{
    row = r;
}

void Position::setCol(int c)
{
    col = c;
}

// Equality operator
bool Position::operator==(const Position &other) const
{
    return row == other.row && col == other.col;
}

// Inequality operator
bool Position::operator!=(const Position &other) const
{
    return !(*this == other);
}
