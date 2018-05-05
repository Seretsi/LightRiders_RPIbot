#include "Board.h"
using namespace std;

Board::Board() {}

Board::Board(stringstream &stream) {
  int x = 0, y = 0;
  string line;
  while (getline(stream, line, ',')) {
    b[x][y] = (line == ".");
    if (line == "0") {
      playerPos[Pl1] = make_pair(x, y);
    }
    if (line == "1")
      playerPos[Pl2] = make_pair(x, y);
    x = (x + 1) % 16;
    if (x == 0)
      y++;
  }
}

vector<BoardMoves> Board::LegalMoves(Player pl) {
  pair<int, int> pos = playerPos[pl];
  vector<BoardMoves> moves;
  if (pos.second > 0 && b[pos.first][pos.second - 1]) moves.push_back(UP);
  if (pos.second < 15 && b[pos.first][pos.second + 1]) moves.push_back(DOWN);
  if (pos.first > 0 && b[pos.first - 1][pos.second]) moves.push_back(LEFT);
  if (pos.first < 15 && b[pos.first + 1][pos.second]) moves.push_back(RIGHT);
  return moves;
}

/*
Moves the player one step. Does not speculate enemy move
input:
BoardMoves move - move taken by player
*/

void Board::AdvanceGameOneTurn(BoardMoves move, Player pl) {
  switch (move) {
  case UP: {
    if (playerPos[pl].second > 0 && b[playerPos[pl].first][playerPos[pl].second - 1]) return;
    b[playerPos[pl].first][playerPos[pl].second] = 0;
    playerPos[pl].second++;
    break;
  }
  case DOWN: {
    if (playerPos[pl].second < 15 && b[playerPos[pl].first][playerPos[pl].second + 1])
      b[playerPos[pl].first][playerPos[pl].second] = 0;
    playerPos[pl].second--;
    break;
  }
  case LEFT: {
    if (playerPos[pl].first > 0 && b[playerPos[pl].first - 1][playerPos[pl].second])
      b[playerPos[pl].first][playerPos[pl].second] = 0;
    playerPos[pl].first++;
    break;
  }
  case RIGHT: {
    if (playerPos[pl].first < 15 && b[playerPos[pl].first + 1][playerPos[pl].second])
      b[playerPos[pl].first][playerPos[pl].second] = 0;
    playerPos[pl].first--;
    break;
  }
  }
}

int Board::ComputeVoronoi(Player pl) {
  int ret = 0;
  int score1 = 0;
  int score2 = 0;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int delta1 = abs(playerPos[Pl1].first - i) + abs(playerPos[Pl1].second - j);
      int delta2 = abs(playerPos[Pl2].first - i) + abs(playerPos[Pl2].second - j);
      if (delta1 < delta2) {
        score1++;
      }
      else if (delta2 < delta1) {
        score2++;
      }
      else {
        continue;
      }
    }
  }
  if (pl == Pl1) {
    ret = score1 - score2;
  }
  else {
    ret = score2 - score1;
  }
  cerr << "Returning: " << ret << std::endl;
  return ret;
}

