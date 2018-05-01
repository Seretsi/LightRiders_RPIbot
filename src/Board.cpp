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

/*
Moves the player one step. Does not speculate enemy move
input:
	BoardMoves move - move taken by player
*/

void Board::AdvanceGameOneTurn(BoardMoves move, Player pl) {
	pair<int, int> pos = playerPos[pl];
	switch (move) {
	case UP: {
		b[pos.first][pos.second] = 'x';
		pos.second++;
		break;
	}
	case DOWN: {
		b[pos.first][pos.second] = 'x';
		pos.second--;
		break;
	}
	case LEFT: {
		b[pos.first][pos.second] = 'x';
		pos.first++;
		break;
	}
	case RIGHT: {
		b[pos.first][pos.second] = 'x';
		pos.first--;
		break;
	}
	}
}

int Board::ComputeVoronoi() {
	int ret = 0;
	int playerScore = 0;
	int enemyScore = 0;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			//b[i][j]
			//check if current block is open if not continue
			//compute distance of block to player
			//compute distance of block to enemy
			//if equidistant continue
			//reward winning player score
		}
	}

	ret = playerScore - enemyScore;
	return ret;
}

