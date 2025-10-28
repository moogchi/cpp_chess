#include "move.h"

using std::string;

string move_to_string(const Move &move) {
  string str = "";

  char fromColumn = (move.from % 8) + 'a';
  char fromRow = (move.from / 8) + '1';

  char toColumn = (move.to % 8) + 'a';
  char toRow = (move.to / 8) + '1';

  str += fromColumn;
  str += fromRow;
  str += toColumn;
  str += toRow;

  // check if promotion
  if (move.promotion_piece != EMPTY) {
    switch (move.promotion_piece) {
    case W_QUEEN:
    case B_QUEEN:
      str += 'q';
      break;

    case W_ROOK:
    case B_ROOK:
      str += 'r';
      break;

    case W_BISHOP:
    case B_BISHOP:
      str += 'b';
      break;

    case W_KNIGHT:
    case B_KNIGHT:
      str += 'n';
      break;
    default:
      break;
    }
  }

  return str;
}