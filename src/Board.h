#ifndef BOARD_h
#define BOARD_h

#include <sstream>
#include <iostream>
#include <vector>
#include "GameEnums.h"

/*
Cell types:
. == empty cell
x == wall from players
0 == pos of player 0
1 == pos of player 1
*/

class Board {
public:
  std::pair<int, int> playerPos[2];
  char b[16][16];
  Board();
  Board(std::stringstream &stream);
  std::vector<BoardMoves> LegalMoves(Player pl);
  void AdvanceGameOneTurn(BoardMoves move, Player pl );
  int ComputeVoronoi(Player pl);
};

#endif
