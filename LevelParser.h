#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <fstream>
#include <string>

using namespace std;

class LevelParser {

 public: 
  LevelParser ();
  ~LevelParser ();

  void parse (const char* filename);

 private:
  ifstream file;
  char commentChar;

  bool getNextLine (string line);

};

#endif
