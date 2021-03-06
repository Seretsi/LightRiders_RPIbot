#ifndef PARSER_h
#define PARSER_h

#include <cstdint>
#include <iostream>
#include <sstream>
#include <queue>
#include "Bot.h"
#include "GameEnums.h"

class Parser {
public:
  Parser(Bot &bot);
  void Parse();
private:
  Bot bot;
  int width = 0;
  int height = 0;
  void ProcessCommand();
  void ProcessAction();
  void ProcessUpdate();
  void ProcessSettings();
  void ProcessSaveBotData();
  std::string NextCmd();
  std::stringstream cmdLine;
};

#endif
