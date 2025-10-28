#include "board.h"
#include "move.h"
#include <iostream>

using std::abs;
using std::cout;

struct Move;

constexpr int piece_values[13] = {
    100,  300, 300, 500, 900, 0, -100, // B_
    -300,                              // B_KNIGHT
    -300,                              // B_BISHOP
    -500,                              // B_QUEEN
    -900,                              // B_QUEEN
    0,                                 // B_KING
    0                                  // EMPTY
};

char get_piece_char(Piece p) {
  switch (p) {
  case W_PAWN:
    return 'P';
  case W_KNIGHT:
    return 'N';
  case W_BISHOP:
    return 'B';
  case W_ROOK:
    return 'R';
  case W_QUEEN:
    return 'Q';
  case W_KING:
    return 'K';
  case B_PAWN:
    return 'p';
  case B_KNIGHT:
    return 'n';
  case B_BISHOP:
    return 'b';
  case B_ROOK:
    return 'r';
  case B_QUEEN:
    return 'q';
  case B_KING:
    return 'k';
  case EMPTY:
    return '.';
  }
  // shouldn't happen
  return '?';
}

Board::Board() {
  for (int i = 0; i < 64; ++i) {
    pieces[i] = EMPTY;
  }
  // white main pieces
  pieces[0] = W_ROOK;
  pieces[1] = W_KNIGHT;
  pieces[2] = W_BISHOP;
  pieces[3] = W_QUEEN;
  pieces[4] = W_KING;
  pieces[5] = W_BISHOP;
  pieces[6] = W_KNIGHT;
  pieces[7] = W_ROOK;

  // white pawns
  for (int i = 8; i < 16; ++i) {
    pieces[i] = W_PAWN;
  }

  // black main pieces
  pieces[56] = B_ROOK;
  pieces[57] = B_KNIGHT;
  pieces[58] = B_BISHOP;
  pieces[59] = B_QUEEN;
  pieces[60] = B_KING;
  pieces[61] = B_BISHOP;
  pieces[62] = B_KNIGHT;
  pieces[63] = B_ROOK;

  // black pawns
  for (int i = 48; i < 56; ++i) {
    pieces[i] = B_PAWN;
  }

  side_to_move = WHITE;                // Start with white
  en_passant_square = -1;              // No enpassant_square at the start
  castling_rights = WK | WQ | BK | BQ; // All rights should be available (1111)
}

void Board::print_board() {
  cout << "\n  +-----------------+\n";
  for (int row = 7; row >= 0; --row) {
    cout << row + 1 << " | ";

    for (int column = 0; column < 8; ++column) {
      int square_index = row * 8 + column;
      cout << get_piece_char(pieces[square_index]) << ' ';
    }
    cout << '|' << '\n';
  }
  cout << "  +-----------------+" << '\n';
  cout << "    a b c d e f g h" << '\n';

  // game state
  cout << "\nSide to move: " << (side_to_move == WHITE ? "White" : "Black")
       << '\n';
  cout << "Castling: " << (castling_rights & WK ? "K" : "-") // bitwise check
       << (castling_rights & WQ ? "Q" : "-")
       << (castling_rights & BK ? "k" : "-")
       << (castling_rights & BQ ? "q" : "-") << '\n';
  cout << "En Passant: "
       << (en_passant_square == -1 ? "none" : std::to_string(en_passant_square))
       << '\n';
}

Side Board::get_piece_side(Piece p) const {
  if (p >= W_PAWN && p <= W_KING) {
    return WHITE;
  } else if (p >= B_PAWN && p <= B_KING) {
    return BLACK;
  }

  return (Side)-1;
}

bool Board::is_our_piece(Piece p) const {
  return get_piece_side(p) == side_to_move;
}

bool Board::is_opponent_piece(Piece p) const {
  Side piece_side = get_piece_side(p);
  return piece_side != (Side)-1 && piece_side != side_to_move;
}

void Board::generate_pseudo_legal_moves(std::vector<Move> &moves) const {
  moves.clear();
  for (int square = 0; square < 64; ++square) {
    Piece p = pieces[square];
    if (p == EMPTY ||
        !is_our_piece(p)) { // skip if the piece on the square is empty
      continue;
    }
    switch (p) {
    case W_PAWN:
    case B_PAWN:
      generate_pawn_moves(square, moves);
      break;

    case W_KNIGHT:
    case B_KNIGHT:
      generate_knight_moves(square, moves);
      break;

    case W_KING:
    case B_KING:
      generate_king_moves(square, moves);
      break;

    case W_ROOK:
    case B_ROOK:
    case W_BISHOP:
    case B_BISHOP:
    case W_QUEEN:
    case B_QUEEN:
      generate_sliding_moves(square, moves);
      break;
    default:
      break;
    }
  }
}

void Board::add_pawn_move(int from, int to, std::vector<Move> &moves) const {
  int promotion_rank = (side_to_move == WHITE) ? 7 : 0;
  int to_row = to / 8;

  if (to_row == promotion_rank) {
    moves.push_back(
        Move(from, to, (side_to_move == WHITE ? W_QUEEN : B_QUEEN)));
    moves.push_back(Move(from, to, (side_to_move == WHITE ? W_ROOK : B_ROOK)));
    moves.push_back(
        Move(from, to, (side_to_move == WHITE ? W_BISHOP : B_BISHOP)));
    moves.push_back(
        Move(from, to, (side_to_move == WHITE ? W_KNIGHT : B_KNIGHT)));
  } else { // regular move
    moves.push_back(Move(from, to));
  }
}

void Board::generate_pawn_moves(int square, std::vector<Move> &moves) const {
  int dir = (side_to_move == WHITE) ? 1 : -1;
  int start_row = (side_to_move == WHITE) ? 1 : 6;
  int current_row = square / 8;
  int current_column = square % 8;

  // Single square move
  int single_move = square + 8 * dir;
  if (single_move > -0 && single_move < 64 && pieces[single_move] == EMPTY) {
    add_pawn_move(square, single_move, moves);

    // check if pawn can move two spots

    if (current_row == start_row) {
      int double_move = square + 16 * dir;
      if (pieces[double_move] == EMPTY) {
        moves.push_back(Move(square, double_move));
      }
    }
  }

  // capturing
  int capture_left = square + 8 * dir - 1;
  if (capture_left >= 0 && capture_left < 64 &&
      (capture_left % 8) == current_column - 1) {
    if (is_opponent_piece(pieces[capture_left])) {
      add_pawn_move(square, capture_left, moves);
    }
  }

  int capture_right = square + 8 * dir + 1;
  if (capture_right >= 0 && capture_right < 64 &&
      (capture_right % 8) == current_column - 1) {
    if (is_opponent_piece(pieces[capture_right])) {
      add_pawn_move(square, capture_right, moves);
    }
  }
}

void Board::generate_knight_moves(int square, std::vector<Move> &moves) const {
  int from_row = square / 8;
  int from_column = square % 8;

  // diagonal movement
  constexpr int offsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};

  for (int offset : offsets) {
    int to_square = square + offset;
    if (to_square < 0 || to_square > 63) {
      continue;
    }
    // check if its wrapping around ie) going from 16 -> 22
    int to_row = to_square / 8;
    int to_column = to_square % 8;

    int row_diff = abs(to_row - from_row);
    int column_diff = abs(to_column - from_column);

    if (!((row_diff == 2 && column_diff == 1) ||
          (row_diff == 1 && column_diff == 2))) {
      continue;
    }

    if (!is_our_piece(pieces[to_square])) {
      moves.push_back(Move(square, to_square));
    }
  }
}

void Board::generate_king_moves(int square, std::vector<Move> &moves) const {
  int from_row = square / 8;
  int from_column = square % 8;

  // one square movements both diagonal and straight
  constexpr int offsets[8] = {-9, -8, -7, -1, 1, 7, 8, 9};

  for (int offset : offsets) {
    int to_square = square + offset;
    if (to_square < 0 || to_square > 63) {
      continue;
    }
    // check if its wrapping around ie) going from 16 -> 22
    int to_row = to_square / 8;
    int to_column = to_square % 8;

    int row_diff = abs(to_row - from_row);          // diff of row
    int column_diff = abs(to_column - from_column); // diff or column

    // check if wraping
    if (row_diff > 1 ||
        column_diff >
            1) { // row diff and column diff should not be greater than 1
      continue;
    }

    if (!is_our_piece(pieces[to_square])) {
      moves.push_back(Move(square, to_square));
    }
  }
}

void Board::generate_sliding_moves(int square, std::vector<Move> &moves) const {
  Piece p = pieces[square];

  int start_dir = 0;
  int end_dir = 8;

  if (p == W_ROOK || p == B_ROOK) {
    start_dir = 0;
    end_dir = 4;
  } else if (p == W_BISHOP || p == B_BISHOP) {
    start_dir = 4;
    end_dir = 8;
  }

  constexpr int all_offsets[8] = {-8, -1, 1, 8, -9, -7, 7, 9};

  for (int i = start_dir; i < end_dir; ++i) {
    int offset = all_offsets[i];
    int to_square = square;

    while (true) {
      to_square += offset;
      if (to_square < 0 || to_square > 63) {
        break;
      } // off the board

      int prev_square = to_square - offset;
      int prev_row = prev_square / 8;
      int prev_column = prev_square % 8;
      int to_row = to_square / 8;
      int to_column = to_square % 8;
      int row_diff = abs(to_row - prev_row);
      int column_diff = abs(to_column - prev_column);

      if (offset == -1 || offset == 1) {
        if (row_diff != 0) {
          break;
        }
      } else if (offset == -8 || offset == 8) {
        if (column_diff != 0) {
          break;
        }
      } else {
        if (row_diff != 1 || column_diff != 1) {
          break;
        }
      }

      if (is_our_piece(pieces[to_square])) {
        break;
      }

      moves.push_back(Move(square, to_square));

      if (is_opponent_piece(pieces[to_square])) {
        break;
      }
    }
  }
}

int Board::evaluate() const {
  int score = 0;
  for (int i = 0; i < 64; ++i) {
    score += piece_values[pieces[i]];
  }
  return score;
}