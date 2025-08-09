#ifndef POSITION_H
#define POSITION_H

class Position
{
private:
    int row;
    int col;

public:
    Position(int r, int c) : row(row), col(col) {}

    // Getters for row and column
    int getRow() const;
    int getCol() const;

    // Setters for row and column
    void setRow(int r);
    void setCol(int c);

    bool operator==(const Position &other) const;
    bool operator!=(const Position &other) const;
};

#endif // POSITION_H