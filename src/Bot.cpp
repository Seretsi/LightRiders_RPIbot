#include "Bot.h"
using namespace std;

Bot::Bot() : board() {
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
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
void Bot::Update(Board board) { this->board = board; };

//total time resources available per round for computation
void Bot::Timebank(int time) { this->timebank = time; };

// Time restored to time bank per round
void Bot::TimePerMove(int time) { this->timebank += time; };

void Bot::YourBot(string name) { };
void Bot::YourBotId(Player playerId) { this->playerId = playerId;  };
void Bot::FieldWidth(int width) { this->width = width; };
void Bot::FieldHeight(int height) { this->height = height; };
void Bot::PlayerNames(string player1, string player2) { };
