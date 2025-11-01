#include "board.h"
#include "move.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using std::abs;
using std::cout;

struct Move;

constexpr int INFINITY_SCORE = 1000000;
constexpr int CHECKMATE_SCORE = 999999;

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
      (capture_right % 8) == current_column + 1) {
    if (is_opponent_piece(pieces[capture_right])) {
      add_pawn_move(square, capture_right, moves);
    }
  }

  // en passant
  if (en_passant_square != -1) {
    if (capture_left == en_passant_square) {
      moves.push_back(Move(square, en_passant_square));
    }
    if (capture_right == en_passant_square) {
      moves.push_back(Move(square, en_passant_square));
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

  // castling
  if (side_to_move == WHITE && square == 4) {
    // white kingside
    if ((castling_rights & WK) && pieces[5] == EMPTY && pieces[6] == EMPTY) {
      moves.push_back(Move(4, 6));
    }
    // white queenside
    if ((castling_rights & WQ) && pieces[1] == EMPTY && pieces[2] == EMPTY &&
        pieces[3] == EMPTY) {
      moves.push_back(Move(4, 2));
    }
  } else if (side_to_move == BLACK && square == 60) {
    // black kingside
    if ((castling_rights & BK) && pieces[61] == EMPTY && pieces[62] == EMPTY) {
      moves.push_back(Move(60, 62));
    }
    // black queenside
    if ((castling_rights & BQ) && pieces[57] == EMPTY && pieces[58] == EMPTY &&
        pieces[59] == EMPTY) {
      moves.push_back(Move(60, 58));
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

BoardState Board::make_move(Move m) {

  // state objects to store info to for unmake move function
  BoardState prev_state;
  prev_state.captured_piece = EMPTY;
  prev_state.en_passant_square = en_passant_square;
  prev_state.castling_rights = castling_rights;

  // move details
  int from = m.from;
  int to = m.to;
  Piece p = pieces[from];
  Piece captured = pieces[to];

  pieces[to] = p;
  pieces[from] = EMPTY;

  if (captured != EMPTY) {
    prev_state.captured_piece = captured;
  }

  if (m.promotion_piece != EMPTY) {
    pieces[to] = m.promotion_piece;
  }

  en_passant_square = -1;

  // en passant brh
  if (p == W_PAWN || p == B_PAWN) {
    if (to == prev_state.en_passant_square) {
      // This IS an en passant capture
      int capture_square;
      if (side_to_move == WHITE) {
        capture_square = to - 8; // Black pawn is 1 rank below
      } else {
        capture_square = to + 8; // White pawn is 1 rank above
      }
      prev_state.captured_piece = pieces[capture_square]; // Store captured pawn
      pieces[capture_square] = EMPTY;                     // Remove it
    } else if (std::abs(to - from) == 16) {
      // This is a double pawn push, set the en passant square
      if (side_to_move == WHITE) {
        en_passant_square = from + 8;
      } else {
        en_passant_square = from - 8;
      }
    }
  }
  // i hate en passant

  // castling
  if ((p == W_KING || p == B_KING) && std::abs(from - to) == 2) {
    switch (to) {
    case 6:
      pieces[5] = pieces[7];
      pieces[7] = EMPTY;
      break;
    case 2:
      pieces[3] = pieces[0];
      pieces[0] = EMPTY;
      break;
    case 62:
      pieces[61] = pieces[63];
      pieces[63] = EMPTY;
      break;
    case 58:
      pieces[59] = pieces[56];
      pieces[56] = EMPTY;
      break;
    }
  }

  // bitwise castlign right check
  if (p == W_KING) {
    castling_rights &= ~(WK | WQ);
  } else if (p == B_KING) {
    castling_rights &= ~(BK | BQ);
  } else if (from == 0 || to == 0) { // a1 rook move/capture
    castling_rights &= ~WQ;
  } else if (from == 7 || to == 7) { // h1 rook move/capture
    castling_rights &= ~WK;
  } else if (from == 56 || to == 56) { // a8 rook move/capture
    castling_rights &= ~BQ;
  } else if (from == 63 || to == 63) { // h8 rook move/capture
    castling_rights &= ~BK;
  }

  // if rook is captured on the home tile the rights are removed
  if (captured != EMPTY) {
    if (to == 0)
      castling_rights &= ~WQ;
    else if (to == 7)
      castling_rights &= ~WK;
    else if (to == 56)
      castling_rights &= ~BQ;
    else if (to == 63)
      castling_rights &= ~BK;
  }

  side_to_move = (side_to_move == WHITE) ? BLACK : WHITE;

  return prev_state;
}

void Board::unmake_move(Move m, const BoardState &prev_state) {
  int from = m.from;
  int to = m.to;
  Piece p = pieces[to];

  side_to_move = (side_to_move == WHITE) ? BLACK : WHITE;
  en_passant_square = prev_state.en_passant_square;
  castling_rights = prev_state.castling_rights;

  if (m.promotion_piece != EMPTY) {
    p = (side_to_move == WHITE) ? W_PAWN : B_PAWN;
  }

  pieces[from] = p;
  pieces[to] = prev_state.captured_piece;

  if ((p == W_PAWN || p == B_PAWN) && to == prev_state.en_passant_square) {
    pieces[to] = EMPTY;
    int capture_square;
    if (side_to_move == WHITE) {
      capture_square = to - 8;
    } else {
      capture_square = to + 8;
    }
    pieces[capture_square] = prev_state.captured_piece;
  }

  if ((p == W_KING || p == B_KING) && std::abs(from - to) == 2) {
    switch (to) {
    case 6:
      pieces[7] = pieces[5];
      pieces[5] = EMPTY;
      break;
    case 2:
      pieces[0] = pieces[3];
      pieces[3] = EMPTY;
      break;
    case 62:
      pieces[63] = pieces[61];
      pieces[61] = EMPTY;
      break;
    case 58:
      pieces[56] = pieces[59];
      pieces[59] = EMPTY;
      break;
    }
  }
}

bool Board::is_square_attacked(int square, Side attacking_side) const {
  // pawn attacks
  int dir = (attacking_side == WHITE) ? -1 : 1;
  Piece attacker_pawn = (attacking_side == WHITE) ? W_PAWN : B_PAWN;

  int capture_left = square + (8 * dir) - 1;
  int capture_right = square + (8 * dir) + 1;
  int current_col = square % 8;

  if (capture_left >= 0 && capture_left < 64 &&
      (capture_left % 8) == current_col - 1) {
    if (pieces[capture_left] == attacker_pawn)
      return true;
  }
  if (capture_right >= 0 && capture_right < 64 &&
      (capture_right % 8) == current_col + 1) {
    if (pieces[capture_right] == attacker_pawn)
      return true;
  }

  // knight attacks
  Piece attacker_knight = (attacking_side == WHITE) ? W_KNIGHT : B_KNIGHT;
  constexpr int knight_offsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
  int from_row = square / 8;
  int from_col = square % 8;

  for (int offset : knight_offsets) {
    int to_square = square + offset;
    if (to_square < 0 || to_square > 63)
      continue;

    int to_row = to_square / 8;
    int to_col = to_square % 8;
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);
    if (!((row_diff == 2 && col_diff == 1) ||
          (row_diff == 1 && col_diff == 2))) {
      continue;
    }

    if (pieces[to_square] == attacker_knight)
      return true;
  }

  // sliding attacks (rook, bishop, queen)
  Piece attacker_rook = (attacking_side == WHITE) ? W_ROOK : B_ROOK;
  Piece attacker_bishop = (attacking_side == WHITE) ? W_BISHOP : B_BISHOP;
  Piece attacker_queen = (attacking_side == WHITE) ? W_QUEEN : B_QUEEN;

  constexpr int all_offsets[8] = {-8, -1, 1, 8, -9, -7, 7, 9};

  for (int i = 0; i < 8; ++i) {
    int offset = all_offsets[i];
    int to_square = square;

    while (true) {
      to_square += offset;
      if (to_square < 0 || to_square > 63)
        break;

      int prev_square = to_square - offset;
      int prev_row = prev_square / 8;
      int prev_col = prev_square % 8;
      int to_row = to_square / 8;
      int to_col = to_square % 8;
      int row_diff = abs(to_row - prev_row);
      int col_diff = abs(to_col - prev_col);

      if (offset == -1 || offset == 1) {
        if (row_diff != 0)
          break;
      } else if (offset == -8 || offset == 8) {
        if (col_diff != 0)
          break;
      } else {
        if (row_diff != 1 || col_diff != 1)
          break;
      }

      Piece p_on_square = pieces[to_square];

      if (p_on_square != EMPTY) {
        if (get_piece_side(p_on_square) == attacking_side) {
          if (i < 4) {
            if (p_on_square == attacker_rook || p_on_square == attacker_queen) {
              return true;
            }
          } else {
            if (p_on_square == attacker_bishop ||
                p_on_square == attacker_queen) {
              return true;
            }
          }
        }
        break;
      }
    }
  }

  // king attacks
  Piece attacker_king = (attacking_side == WHITE) ? W_KING : B_KING;
  constexpr int king_offsets[8] = {-9, -8, -7, -1, 1, 7, 8, 9};
  from_row = square / 8;
  from_col = square % 8;

  for (int offset : king_offsets) {
    int to_square = square + offset;
    if (to_square < 0 || to_square > 63)
      continue;

    int to_row = to_square / 8;
    int to_col = to_square % 8;
    int row_diff = abs(to_row - from_row);
    int col_diff = abs(to_col - from_col);
    if (row_diff > 1 || col_diff > 1)
      continue;

    if (pieces[to_square] == attacker_king)
      return true;
  }

  return false;
}

bool Board::is_in_check() const {
  Piece our_king = (side_to_move == WHITE) ? W_KING : B_KING;
  Side opponent_side = (side_to_move == WHITE) ? BLACK : WHITE;
  int king_square = -1;

  for (int i = 0; i < 64; ++i) {
    if (pieces[i] == our_king) {
      king_square = i;
      break;
    }
  }

  if (king_square == -1) {
    return false;
  }

  return is_square_attacked(king_square, opponent_side);
}

void Board::generate_legal_moves(std::vector<Move> &moves) {
  std::vector<Move> pseudo_moves;
  generate_pseudo_legal_moves(pseudo_moves);

  moves.clear();

  for (Move m : pseudo_moves) {
    BoardState state = make_move(m);

    if (!is_in_check()) {
      moves.push_back(m);
    }

    unmake_move(m, state);
  }
}

int Board::negamax(int depth, int alpha, int beta) {
  if (depth == 0) {
    return evaluate() * (side_to_move == WHITE ? 1 : -1);
  }

  std::vector<Move> moves;
  generate_legal_moves(moves);

  if (moves.empty()) {
    if (is_in_check()) {
      return -(CHECKMATE_SCORE + depth);
    } else {
      return 0;
    }
  }

  int best_score = -INFINITY_SCORE;

  for (Move m : moves) {
    BoardState state = make_move(m);

    int score = -negamax(depth - 1, -beta, -alpha);

    unmake_move(m, state);

    best_score = std::max(best_score, score);
    alpha = std::max(alpha, best_score);

    if (alpha >= beta) {
      break;
    }
  }

  return best_score;
}

Move Board::find_best_move(int depth) {
  std::vector<Move> moves;
  generate_legal_moves(moves);

  Move best_move;
  int alpha = -INFINITY_SCORE;
  int beta = INFINITY_SCORE;

  for (Move m : moves) {
    BoardState state = make_move(m);

    int score = -negamax(depth - 1, -beta, -alpha);

    unmake_move(m, state);

    if (score > alpha) {
      alpha = score;
      best_move = m;
    }
  }

  return best_move;
}
