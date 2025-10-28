#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include <string>

using std::string;

struct Move {
  int from; // index 0-63
  int to;   // index 0-63
  Piece promotion_piece;

  // Normal Move Constructor
  Move(int from, int to) : from(from), to(to), promotion_piece(EMPTY) {}

  // Promotion Move Constructor
  Move(int from, int to, Piece promo)
      : from(from), to(to), promotion_piece(promo) {}
};

string move_to_string(const Move &move);
#endif