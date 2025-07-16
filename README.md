# GOMOKU_AI

A Gomoku (五子棋) AI player written in C++, using Minimax and Alpha-Beta pruning with a handcrafted heuristic evaluation function.

## Features

- 🎯 **AI Strategy:**  
  Uses Minimax search with Alpha-Beta pruning to efficiently explore move possibilities.

- 🧠 **Evaluation Function:**  
  Evaluates the board by considering:
  - Connected pieces (2, 3, 4 in a row)
  - Open ends (live threes/fours)
  - Opponent blocking
  - Double threats and traps

- 📍 **Candidate Move Pruning:**  
  Only considers moves near existing pieces using a `checkSurrounding()` method to reduce unnecessary calculations.

- 🏁 **First Move Strategy:**  
  Automatically plays in the center (天元) or nearby if it's the first move.

## File Structure

- `main()` – reads board, initializes state, writes output.
- `State` – represents a board state with helper methods for candidates and evaluation.
- `Minimax()` – core decision algorithm.
- `evaluate_score()` – heuristic function evaluating game board.
- `checkSurrounding()` – restricts search space for better performance.

## How to Run

Compile and run:

```bash
g++ -std=c++11 -O2 -o gomoku gomoku.cpp
./gomoku input.txt output.txt
