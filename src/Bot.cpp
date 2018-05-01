#include "Bot.h"
#include <fstream>
#include <string>
#include <bitset>
#include <assert.h>
using namespace std;

#define DEBUG 1

Bot::Bot() : board() {
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

  numBitsPerState = WINDOW_HEIGHT * WINDOW_WIDTH + (storeOpponent) ? numBitsForOpponent : 0;
}

long long Bot::getStateValue(Board board) {

  assert (WINDOW_WIDTH == WINDOW_HEIGHT); //this breaks if this is true lmfao, to fix just define separate xend and yend and adjust for left/right

  unsigned long long rep = 0;

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

  if (DEBUG) cerr << "Located at ("<<board.playerPos[playerId].first<<","<<board.playerPos[playerId].second<<
    ")\nField of View: ("<<xstart<<","<<ystart<<") through ("<<xstart + (WINDOW_WIDTH-1)*xdir<<","<<ystart + (WINDOW_HEIGHT-1)*ydir<<"):"<<endl;

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
      for (int y = ystart; (ydir == 1) ? y < ystart + WINDOW_HEIGHT : y > ystart - WINDOW_HEIGHT; y += ydir){
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

  if (DEBUG) cerr << "State maps to " << rep <<"\n("<<bitset<64>{rep}<<") "<<endl;

  return rep;
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

void Bot::WriteQTable(std::string address) {
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
  vector<BoardMoves> moves = board.LegalMoves(playerId);
  if(moves.size() == 0){
    MakeMove(UP);
    return;
  }

  if (DEBUG) getStateValue(board);

  MakeMove(moves[rand() % moves.size()]); // we need to replace this line here.
  /*From here we have access so far to the number a list of moves we can make
  our AI needs to pick one here. 
  
  Each move/choice is our action* 
  
  we could launch a group of threads that each operate on the available next actions optimising for 
  their own goal
  std::thread agent(foo(*val));
  NOTE: The more I look into this, its seem like a lot of work

  To print to a debug terminal during game play we need to print to std::cerr I think
  i.e std::cerr << printboard();
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
