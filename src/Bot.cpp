#include "Bot.h"
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <bitset>
#include <assert.h>
#include <math.h>
#include <vector>
using namespace std;

#define DEBUG 1
#define NEWSTATEREP 0

Bot::Bot() : board() {
  if(NEWSTATEREP) cerr << "V5 Starting Voronoi with Q Learning: Simple Rep" << endl;
  else cerr << "V5 Starting Voronoi with Q Learning: Complex Rep" << endl;
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

  numBitsPerState = WINDOW_HEIGHT * WINDOW_WIDTH + ((storeOpponent) ? numBitsForOpponent : 0);
  InitQTable(TableFile);
}

long long Bot::getStateValue(Board board) {

  unsigned long long rep = 0;

  //assert (WINDOW_WIDTH == WINDOW_HEIGHT); //this breaks if this is true lmfao, to fix just define separate xend and yend and adjust for left/right
  if (NEWSTATEREP) {
    rep += 16 * board.playerPos[(playerId + 1) % 2].first + board.playerPos[(playerId + 1) % 2].second;
  }
  else {
    int xstart, ystart, xdir, ydir;

    switch (lastMove) {
    case UP:
      xstart = board.playerPos[playerId].first - PER_OFFSET;
      ystart = board.playerPos[playerId].second - PAR_OFFSET;
      xdir = 1;
      ydir = 1;
      break;
    case DOWN:
      xstart = board.playerPos[playerId].first + PER_OFFSET;
      ystart = board.playerPos[playerId].second + PAR_OFFSET;
      xdir = -1;
      ydir = -1;
      break;
    case RIGHT:
      xstart = board.playerPos[playerId].first + PAR_OFFSET;
      ystart = board.playerPos[playerId].second - PER_OFFSET;
      xdir = -1;
      ydir = 1;
      break;
    case LEFT:
      xstart = board.playerPos[playerId].first - PAR_OFFSET;
      ystart = board.playerPos[playerId].second + PER_OFFSET;
      xdir = 1;
      ydir = -1;
    }

    if (DEBUG) cerr << "Located at (" << board.playerPos[playerId].first << "," << board.playerPos[playerId].second <<
      ")\nField of View: (" << xstart << "," << ystart << ") through (" << xstart + (WINDOW_WIDTH - 1)*xdir << "," << ystart + (WINDOW_HEIGHT - 1)*ydir << "):" << endl;

    //store your position
    if (lastMove == UP || lastMove == DOWN) {
      for (int y = ystart; (ydir == 1) ? y < ystart + WINDOW_HEIGHT : y > ystart - WINDOW_HEIGHT; y += ydir) {
        for (int x = xstart; (xdir == 1) ? x < xstart + WINDOW_WIDTH : x > xstart - WINDOW_WIDTH; x += xdir) {
          rep <<= 1;
          if (y < 0 || y > 15 || x < 0 || x > 15) {
            if (DEBUG) cerr << "/";
            continue;
          }
          rep += board.b[x][y]; //If obstacle, add 0 bit. If traversable, add 1 bit.
          if (DEBUG) {
            if (board.b[x][y]) cerr << ".";
            else cerr << "x";
          }
        }
        if (DEBUG) cerr << endl;
      }
    }
    else {
      for (int x = xstart; (xdir == 1) ? x < xstart + WINDOW_WIDTH : x > xstart - WINDOW_WIDTH; x += xdir) {
        for (int y = ystart; (ydir == 1) ? y < ystart + WINDOW_HEIGHT : y > ystart - WINDOW_HEIGHT; y += ydir) {
          rep <<= 1;
          if (y < 0 || y > 15 || x < 0 || x > 15) {
            if (DEBUG) cerr << "/";
            continue;
          }
          rep += board.b[x][y]; //If obstacle, add 0 bit. If traversable, add 1 bit.
          if (DEBUG) {
            if (board.b[x][y]) cerr << ".";
            else cerr << "x";
          }
        }
        if (DEBUG) cerr << endl;
      }
    }

    //store opponent's position
    if (storeOpponent) {
      rep <<= numBitsForOpponent;
      rep += 16 * board.playerPos[(playerId + 1) % 2].first + board.playerPos[(playerId + 1) % 2].second;
    }

    if (DEBUG) cerr << "State maps to " << rep << "\n(" << bitset<64>{rep} << ") " << endl;
  }
  return rep;
}

//needs to be double check and given a run signal from reading server messages
void Bot::InitQTable(std::string address) {
  if (numActionsPerState != 0 && numStates != 0) return;
	//load or generate qTable
	ifstream qTableFile;
	qTableFile.open(address);
	if (qTableFile) {
    cerr << "Loading Q table from file " << TableFile << endl;
    qTableFile >> numStates >> numActionsPerState;
    cerr <<"size: "<<numStates<<" by "<< numActionsPerState<< endl;

		qTable = new float*[numActionsPerState];
		for (int i = 0; i < numActionsPerState; i++) {
			qTable[i] = new float[numStates];
		}
		for (int i = 0; i < numActionsPerState; i++) {
			for (int j = 0; j < numStates; j++) {
        qTableFile >> qTable[i][j];
			}
		}
	}
	else {
    cerr << "Could not load Q table from " << TableFile << endl;
    numActionsPerState = 4;
    if (NEWSTATEREP) { numStates = 16 * 16; }//width * height;
    else { numStates = (int)pow(2, numBitsPerState); }
    cerr << "Creating new table of size " << numStates << " by " << numActionsPerState << endl;
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

  for (int i = 0; i < numActionsPerState; i++) {
    for (int j = 0; j < numStates; j++) {
      cerr << qTable[i][j] << " ";
    }
  }cerr << endl;

	qTableFile.close();
}

void Bot::SaveQTable(std::string address) {
  cerr << "Save Q table to file " << TableFile << endl;
	ofstream qTableFile;
	qTableFile.open(address);
  qTableFile << numStates<< " "<< numActionsPerState << endl;

	for (int i = 0; i < numActionsPerState; i++) {
		for (int j = 0; j < numStates; j++) {
      qTableFile << qTable[i][j] << " ";
		}
		qTableFile << endl;
	}
  qTableFile.close();
}

// instructions for the player take the form of string outputs to the engine
// functions is essential good for now

void Bot::MakeMove(BoardMoves boardMove) { 
  lastMove = boardMove;
  switch (boardMove) {
    case UP:    printf("up\n"); break;
    case DOWN:  printf("down\n"); break;
    case LEFT:  printf("left\n"); break;
    case RIGHT: printf("right\n"); break;
  }
}

void Bot::Move(int time) {

  SaveQTable(TableFile);
  vector<BoardMoves> moves = board.LegalMoves(playerId);
  if(moves.size() == 0){
    MakeMove(UP);
    return;
  }
  
  float alpha = 0.4f;
  float gamma = 0.8f;
  float r = 0.2f;

  map<BoardMoves, float> nextMove;

  // walk through available next moves and computer QValue of each option
  // (I hope I understand our approach and am going the right direction)
  int s = getStateValue(board);
  for (vector<BoardMoves>::iterator it = moves.begin(); it != moves.end(); it++) {
	  Board nextBoard = board;
	  nextBoard.AdvanceGameOneTurn(*it, playerId);
    lastMove = *it;//important for getting state in sprime
    int sPrime = getStateValue(nextBoard);
    int voronoi = board.ComputeVoronoi(playerId);
    //int simpleReward = nextBoard.LegalMoves(playerId).size() * 4 - 8;
	  vector<BoardMoves> futureMoves = nextBoard.LegalMoves(playerId);
	  map<BoardMoves, int> nextMoveProbabilities; //probabilities defines by voronoi scores (rand pick policy);
    float totalScoreForRandPolicy = 9999;//0.0f;
    float futureQ = 0.0f;
	  for (auto& futureMove : futureMoves) {
      if (futureQ < qTable[futureMove][sPrime]) futureQ = qTable[futureMove][sPrime];
		  /**Board nextNextBoard = nextBoard;
		  nextNextBoard.AdvanceGameOneTurn(futureMove, playerId);
		  nextMoveProbabilities[futureMove] = nextBoard.LegalMoves(playerId).size() + nextNextBoard.LegalMoves(playerId).size(); //simple reward, replaces nextNextBoard.ComputeVoronoi(playerId);
      totalScoreForRandPolicy += nextMoveProbabilities[futureMove]; **/
	  }
	  float randSelectionProbability = float(rand() / RAND_MAX); // value 0 - 1
    int randomizedFutureQ = 0.0f;
	  float sum = 0.0f;
	  for (auto& futureMove : futureMoves) {
		  sum += nextMoveProbabilities[futureMove] / totalScoreForRandPolicy;
		  if (randSelectionProbability <= sum) {
        randomizedFutureQ = nextMoveProbabilities[futureMove];
			  break;
		  }
	  }
    cerr << "r: "<<voronoi << "fqr:" << randomizedFutureQ << "fq:" << futureQ << "Q[" << s << "][" << *it << "]=" << qTable[*it][s] << " ";
	  // Q[s,a] = Q[s,a] + alpha((r + gamma * maxQ[s',a']) - Q[s,a])
    qTable[*it][s] = qTable[*it][s] + alpha * (((float)voronoi + gamma * futureQ) - qTable[*it][s]);
    cerr << " to " << qTable[*it][s] << endl;
    //used for move selection
    nextMove[*it] = qTable[*it][s] + alpha * (((float)voronoi + gamma * futureQ) - qTable[*it][s]);
  }

  // select the best move from computed Q values of available next moves (actions)
  float highestVal = -1 * INFINITY;
  std::vector<BoardMoves> bestMoves;
  for (auto& best : nextMove) {
	  if (best.second > highestVal) {
      highestVal = best.second;
      bestMoves.clear();
		  bestMoves.push_back(best.first);
    }
    else if (highestVal == best.second) {
      bestMoves.push_back(best.first);
    }

  }

  MakeMove(bestMoves[rand() % bestMoves.size()]); //make move randomly from among best
  //MakeMove(moves[rand() % moves.size()]); // we need to replace this line here.  
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
