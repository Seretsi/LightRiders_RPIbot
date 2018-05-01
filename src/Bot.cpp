#include "Bot.h"
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
using namespace std;

Bot::Bot() : board() {
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

//needs to be double check and given a run signal from reading server messages
void Bot::InitQTable(std::string address) {
	//load or generate qTable
	fstream qTableFile;
	qTableFile.open(address);
	if (qTableFile) {
		cin >> numStates >> numActionsPerState;
		qTable = new float*[numActionsPerState];
		for (int i = 0; i < numActionsPerState; i++) {
			qTable[i] = new float[numStates];
		}
		for (int i = 0; i < numActionsPerState; i++) {
			for (int j = 0; j < numStates; j++) {
				cin >> qTable[i][j];
			}
		}
	}
	else {
		int numActionsPerState = 4, numStates = width * height;
		qTable = new float*[numActionsPerState];
		for (int i = 0; i < numActionsPerState; i++) {
			qTable[i] = new float[numStates];
		}
		for (int i = 0; i < numActionsPerState; i++) {
			for (int j = 0; j < numStates; j++) {
				qTable[i][j] = 0.0f;
			}
		}
	}
	qTableFile.close();
}

void Bot::SaveQTable(std::string address) {
	fstream qTableFile;
	qTableFile.open(address);
	char buf[7];
	snprintf(buf, 24, "%i %i", numStates, numActionsPerState);
	qTableFile << buf << endl;
	for (int i = 0; i < numActionsPerState; i++) {
		for (int j = 0; j < numStates; j++) {
			qTableFile << qTable[i][j] << " ";
		}
		qTableFile << endl;
	}
	qTableFile.close();
}

int Bot::ComputeVoronoi(Board board) {
	int ret = 0;

	return ret;
}

// instructions for the player take the form of string outputs to the engine
// functions is essential good for now

void Bot::MakeMove(BoardMoves boardMove) { 
  switch (boardMove) {
    case UP:    printf("up\n"); break;
    case DOWN:  printf("down\n"); break;
    case LEFT:  printf("left\n"); break;
    case RIGHT: printf("right\n"); break;
  }
}

void Bot::Move(int time) {
  vector<BoardMoves> moves = board.LegalMoves(playerId);
  if(moves.size() == 0){
    MakeMove(UP);
    return;
  }

  map<BoardMoves, float> nextMove;

  for (vector<BoardMoves>::iterator it = moves.begin(); it != moves.end(); it++) {
	  Board nextBoard = board;
	  
	  float voronoi = ComputeVoronoi(nextBoard);

	  nextMove[*it] = voronoi; // more pieces needed for equation: Q[s,a] = Q[s,a] + alpha((r + gamma * maxQ[s',a']) - Q[s,a])
  }

  // select the best move from computed Q values of available next moves (actions)
  BoardMoves bestMove = nextMove.begin()->first;
  for (map<BoardMoves, float>::iterator it = nextMove.begin(); it != nextMove.end(); it++) {
	  if (nextMove[bestMove] > it->second) {
		  bestMove = it->first;
	  }
  }
  // MakeMove(bestMove);
  MakeMove(moves[rand() % moves.size()]); // we need to replace this line here.
  /*From here we have access so far to the number a list of moves we can make
  our AI needs to pick one here. 
  
  Each move/choice is our action* 
  */

}

void Bot::Round(int time) {  };
void Bot::Update(Board board) { 
	this->prevBoard = this->board;
	this->board = board;
};

//total time resources available per round for computation
void Bot::Timebank(int time) { this->timebank = time; };

// Time restored to time bank per round
void Bot::TimePerMove(int time) { this->timebank += time; };

void Bot::YourBot(string name) { };
void Bot::YourBotId(Player playerId) { this->playerId = playerId;  };
void Bot::FieldWidth(int width) { this->width = width; };
void Bot::FieldHeight(int height) { this->height = height; };
void Bot::PlayerNames(string player1, string player2) { };
