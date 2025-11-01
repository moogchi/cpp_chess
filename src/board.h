#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>

struct Move;

// 0 to 5 are white, 6-11 are black, 12 is empty space
enum Piece {
  W_PAWN,
  W_KNIGHT,
  W_BISHOP,
  W_ROOK,
  W_QUEEN,
  W_KING,
  B_PAWN,
  B_KNIGHT,
  B_BISHOP,
  B_ROOK,
  B_QUEEN,
  B_KING,
  EMPTY
};

struct BoardState {
  Piece captured_piece;
  int en_passant_square;
  int castling_rights;
};

enum Side { WHITE, BLACK };

// will use bitwise operator to check if its possible to castle
constexpr int WK{1};
constexpr int WQ{2};
constexpr int BK{4};
constexpr int BQ{8};

struct Board {
  // 64 element array for the board row*8+column
  Piece pieces[64];

  Side side_to_move;

  int en_passant_square;

  int castling_rights;

  // Constructor to initalize the baord to the correct starting position
  Board();

  // Print the board
  void print_board();
  void generate_pseudo_legal_moves(std::vector<Move> &moves) const;

  Side get_piece_side(Piece p) const;

  int evaluate() const;

  Move find_best_move(int depth);

  BoardState make_move(Move m);
  void unmake_move(Move m, const BoardState &prev_state);

  bool is_in_check() const;

  void generate_legal_moves(std::vector<Move> &moves);

private: // encapsulated function for moves
  void generate_pawn_moves(int square, std::vector<Move> &moves) const;
  void generate_knight_moves(int square, std::vector<Move> &moves) const;
  void generate_king_moves(int square, std::vector<Move> &moves) const;
  void generate_sliding_moves(int square, std::vector<Move> &moves)
      const; // works for rook, bishop, queen
  void add_pawn_move(int from, int to,
                     std::vector<Move> &moves) const; // for promotion

  bool is_our_piece(Piece p) const;
  bool is_opponent_piece(Piece p) const;

  bool is_square_attacked(int square, Side attacking_side) const;

  int negamax(int depth, int alpha, int beta);
};

#endif
