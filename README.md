# C++ Chess Engine

A fully functional chess engine written in C++ with an AI opponent using the Negamax algorithm with alpha-beta pruning.

## Features

- ♟️ **Complete Chess Rules Implementation**

  - All standard piece movements (pawns, knights, bishops, rooks, queens, kings)
  - Special moves: castling (kingside & queenside), en passant, pawn promotion
  - Legal move generation with check/checkmate detection
  - Stalemate detection

- 🤖 **AI Opponent**

  - Negamax search algorithm with alpha-beta pruning
  - Configurable search depth (default: 5 ply)
  - Material-based position evaluation

- 🎮 **Interactive Gameplay**
  - Play as White against the computer (Black)
  - Simple coordinate notation (e.g., `e2e4`)
  - Move validation and error handling
  - Board visualization in terminal

## Building the Project

### Prerequisites

- C++17 compatible compiler (g++, clang++)
- Make

### Compilation

```bash
make
```

### Clean Build

```bash
make clean
make
```

## How to Play

### Running the Game

```bash
./chess_engine
```

### Move Notation

Moves are entered using coordinate notation:

- **Standard move**: `e2e4` (move piece from e2 to e4)
- **Pawn promotion**: `e7e8q` (promote pawn to queen)
  - `q` = Queen
  - `r` = Rook
  - `b` = Bishop
  - `n` = Knight

### Example Game Session

```
  +-----------------+
8 | r n b q k b n r |
7 | p p p p p p p p |
6 | . . . . . . . . |
5 | . . . . . . . . |
4 | . . . . . . . . |
3 | . . . . . . . . |
2 | P P P P P P P P |
1 | R N B Q K B N R |
  +-----------------+
    a b c d e f g h

Side to move: White
Castling: KQkq
En Passant: none
Enter your move (e.g., e2e4): e2e4

Computer is thinking at depth 5...
Computer plays: e7e5
```

## Project Structure

```
cpp_chess/
├── src/
│   ├── board.h          # Board class declaration
│   ├── board.cpp        # Board implementation (move generation, AI)
│   ├── move.h           # Move structure
│   ├── move.cpp         # Move utilities
│   └── main.cpp         # Game loop and user interface
├── makefile             # Build configuration
└── README.md            # This file
```

## Code Architecture

### Core Components

- **Board**: Represents the chess board state

  - 64-element array for piece positions
  - Side to move, castling rights, en passant tracking
  - Move generation (pseudo-legal and legal)
  - Position evaluation

- **Move**: Represents a chess move

  - Source and destination squares (0-63)
  - Promotion piece (if applicable)

- **AI Search**:
  - `negamax()`: Recursive minimax search with alpha-beta pruning
  - `find_best_move()`: Root-level search to find optimal move
  - `evaluate()`: Material-based position scoring

### Key Algorithms

**Move Generation**:

1. Generate pseudo-legal moves for all pieces
2. Filter by making each move and checking if king is in check
3. Unmake move and restore board state

**AI Search**:

1. Negamax search explores game tree to specified depth
2. Alpha-beta pruning cuts off branches that won't affect final decision
3. Position evaluation at leaf nodes using material values

## Customization

### Adjusting AI Strength

Edit `AI_SEARCH_DEPTH` in `src/main.cpp`:

```cpp
constexpr int AI_SEARCH_DEPTH = 5;  // Increase for stronger play
```

- **Depth 3-4**: Fast, beginner level
- **Depth 5-6**: Moderate, intermediate level
- **Depth 7+**: Slow, advanced level

### Piece Values

Edit `piece_values` array in `src/board.cpp`:

```cpp
constexpr int piece_values[13] = {
    100,   // W_PAWN
    300,   // W_KNIGHT
    300,   // W_BISHOP
    500,   // W_ROOK
    900,   // W_QUEEN
    0,     // W_KING
    // ... (negative values for black)
};
```

## Future Enhancements

Possible improvements:

- [ ] Opening book
- [ ] Transposition tables
- [ ] Move ordering (MVV-LVA, killer moves)
- [ ] Quiescence search
- [ ] Iterative deepening
- [ ] Position evaluation improvements (piece-square tables)
- [ ] UCI protocol support
- [ ] Time management
- [ ] Endgame tablebases

## License

This project is open source and available for educational purposes.

## Author

Created as a chess engine learning project.
