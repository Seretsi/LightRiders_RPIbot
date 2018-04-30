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
  int numActionsPerState, numStates;

protected:
  void MakeMove(BoardMoves boardMove);

public:
  Bot();
  //initialization
  void InitQTable(std::string address);
  //finalize
  void WriteQTable(std::string address);
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