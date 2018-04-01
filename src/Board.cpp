#include "Board.h"
using namespace std;

Board::Board() {}

Board::Board(stringstream &stream){
  int x = 0, y = 0;
  string line;
  while (getline(stream, line, ',')){
    b[x][y] = (line == ".");
    if (line == "0"){
      playerPos[Pl1] = make_pair(x,y);
    }
    if (line == "1")
      playerPos[Pl2] = make_pair(x,y);
    x = (x+1)%16;
    if (x == 0)
      y++;
  }
}

vector<BoardMoves> Board::LegalMoves(Player pl){
  pair<int, int> pos = playerPos[pl];
  vector<BoardMoves> moves;
  if (pos.second > 0  && b[pos.first][pos.second-1]) moves.push_back(UP);
  if (pos.second < 15 && b[pos.first][pos.second+1]) moves.push_back(DOWN);
  if (pos.first > 0   && b[pos.first-1][pos.second]) moves.push_back(LEFT);
  if (pos.first < 15  && b[pos.first+1][pos.second]) moves.push_back(RIGHT);
  return moves;
}
