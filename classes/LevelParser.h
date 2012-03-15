#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Cell.h"
#include "Level.h"

using namespace std;

class LevelParser {

 public: 
  LevelParser ();
  ~LevelParser ();

  Level parse (const char* filename, sf::RenderWindow& window);

 private:
  ifstream file;
  char commentChar;

};

#endif
