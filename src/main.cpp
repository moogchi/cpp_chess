#include "board.h"
#include "move.h"
#include <iostream>
#include <vector>

using std::cout;

int main() {
  Board my_board;

  cout << "Initial board" << '\n';

  my_board.print_board();

  cout << "Generating move for white" << '\n';

  std::vector<Move> move_list;

  my_board.generate_pseudo_legal_moves(move_list);

  cout << "Found " << move_list.size() << " pseudolegal moves:" << '\n';

  for (const Move &move : move_list) {
    std::cout << move_to_string(move) << '\n';
  }

  return 0;
}