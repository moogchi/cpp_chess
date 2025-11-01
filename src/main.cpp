#include "board.h"
#include "move.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;

Move parse_move(Board &board, std::string move_str) {
  std::vector<Move> legal_moves;
  board.generate_legal_moves(legal_moves);

  int from_col = move_str[0] - 'a';
  int from_row = move_str[1] - '1';
  int to_col = move_str[2] - 'a';
  int to_row = move_str[3] - '1';

  int from_sq = from_row * 8 + from_col;
  int to_sq = to_row * 8 + to_col;

  Piece promotion_p = EMPTY;
  if (move_str.length() == 5) {
    switch (move_str[4]) {
    case 'q':
      promotion_p = (board.side_to_move == WHITE) ? W_QUEEN : B_QUEEN;
      break;
    case 'r':
      promotion_p = (board.side_to_move == WHITE) ? W_ROOK : B_ROOK;
      break;
    case 'b':
      promotion_p = (board.side_to_move == WHITE) ? W_BISHOP : B_BISHOP;
      break;
    case 'n':
      promotion_p = (board.side_to_move == WHITE) ? W_KNIGHT : B_KNIGHT;
      break;
    }
  }

  for (const Move &m : legal_moves) {
    if (m.from == from_sq && m.to == to_sq) {
      if (m.promotion_piece != EMPTY) {
        if (m.promotion_piece == promotion_p) {
          return m;
        }
      } else {
        return m;
      }
    }
  }

  return Move(-1, -1);
}

int main() {
  Board board;
  std::string move_str;

  constexpr int AI_SEARCH_DEPTH = 5;

  while (true) {
    board.print_board();

    std::vector<Move> legal_moves;
    board.generate_legal_moves(legal_moves);

    if (legal_moves.empty()) {
      if (board.is_in_check()) {
        std::cout << "Checkmate! "
                  << (board.side_to_move == WHITE ? "Black" : "White")
                  << " wins.\n";
      } else {
        std::cout << "Stalemate! It's a draw.\n";
      }
      break;
    }

    if (board.side_to_move == WHITE) {
      std::cout << "Enter your move (e.g., e2e4): ";
      std::cin >> move_str;

      Move user_move = parse_move(board, move_str);

      if (user_move.from == -1) {
        std::cout << "Invalid or illegal move. Try again.\n";
        continue;
      }

      board.make_move(user_move);
    } else {
      std::cout << "\nComputer is thinking at depth " << AI_SEARCH_DEPTH
                << "...\n";

      Move ai_move = board.find_best_move(AI_SEARCH_DEPTH);

      std::cout << "Computer plays: " << move_to_string(ai_move) << "\n";

      board.make_move(ai_move);
    }
  }

  return 0;
}
