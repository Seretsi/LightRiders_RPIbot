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
  std::string TableFile = "./qtable-comrep-voronoi.txt";

  //constants for state representation (window of view size, offset)
  const int WINDOW_HEIGHT = 4; //Height with direction relative to agent direction
  const int WINDOW_WIDTH = 3; //Width with direction relative to agent direction
  const int PAR_OFFSET = 2; //Offset parallel to (same direction as) agent, AKA Y offset if facing north.
  const int PER_OFFSET = 1; //Offset perpendicular to (same direction as) agent, AKA X offset if facing north.
                              //Offset of (0,0) means agent is in upper lefthand corner, (2,2) is center (if width/height is 5)
  const boolean storeOpponent = false; //set to false for simplistic, only storing self bits
  const int numBitsForOpponent = 8; //number of bits required to store opponents position
  int numBitsPerState; //state size in bits

  Player playerId;
  int width, height;
  Board board;
  Board prevBoard;
  int timebank;
  float** qTable;
  BoardMoves lastMove = UP;
  boolean firstMove = true;

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
  //finalize
  void SaveQTable(std::string address);
  //get value representing state from board
  long long getStateValue(Board board);
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