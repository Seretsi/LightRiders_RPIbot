#ifndef BOT_h
#define BOT_h

#include <iostream>
#include <stdio.h>
#include "sys/time.h"
#include <vector>
#include <string>
#include "Board.h"

class Bot {
private:
  Player playerId;
  int width, height;
  Board board;
  Board prevBoard;
  int timebank;
  float** qTable;

  //each block on the board is a state
  // from each block the bot has an option of going to an adjacent block up, down, left or right
  // totalling 4 actions per state, one of which will always be illegal depending on the direction 
  // with which the bot entered that state. In other words, a bot can never return to a state it just left.
  int numActionsPerState, numStates;

protected:
  void MakeMove(BoardMoves boardMove);

public:
  Bot();
  //initialization
  void InitQTable(std::string address);
  //Computation
  int ComputeVoronoi(Board board);
  //finalize
  void SaveQTable(std::string address);
  // Action
  virtual void Move(int time);
  // Update
  virtual void Round(int time);
  virtual void Update(Board board);
  // Settings
  virtual void Timebank(int time);
  virtual void TimePerMove(int time);
  virtual void YourBot(std::string name);
  virtual void YourBotId(Player playerId);
  virtual void FieldWidth(int width);
  virtual void FieldHeight(int height);
  virtual void PlayerNames(std::string player1, std::string player2);
};
#endif