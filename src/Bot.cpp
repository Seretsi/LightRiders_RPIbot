#include "Bot.h"
using namespace std;

Bot::Bot() : board() {
  struct timeval time;
  gettimeofday(&time,NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

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
  MakeMove(moves[rand() % moves.size()]);
}

void Bot::Round(int time) {  };
void Bot::Update(Board board) { this->board = board; };
void Bot::Timebank(int time) { };
void Bot::TimePerMove(int time) { };
void Bot::YourBot(string name) { };
void Bot::YourBotId(Player playerId) { this->playerId = playerId;  };
void Bot::FieldWidth(int width) { this->width = width; };
void Bot::FieldHeight(int height) { this->height = height; };
void Bot::PlayerNames(string player1, string player2) { };
