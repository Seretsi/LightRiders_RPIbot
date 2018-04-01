#ifndef BOARD_h
#define BOARD_h

#include <sstream>
#include <vector>
#include "GameEnums.h"

class Board {
  std::pair<int, int> playerPos[2];
public:
  char b[16][16];
  Board();
  Board(std::stringstream &stream);
  std::vector<BoardMoves> LegalMoves(Player pl);
};

#endif
