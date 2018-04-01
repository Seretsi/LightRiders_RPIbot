#include <cstdint>
#include "src/Parser.h"
#include "src/Bot.h"

int main() {
  Bot bot;
  Parser parser(bot);
  parser.Parse();
  return 0;
}

