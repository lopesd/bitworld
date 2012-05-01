/** LevelParser.h
 *  LevelParser is a static class. It reads texts files and returns constructed Level objects.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <fstream>
#include <string>

#include "Cell.h"
#include "Level.h"

using namespace std;

class LevelParser {

 public:
  // Parse text file into level object
  static Level* Parse (const char* filename, sf::RenderWindow& window);
  static vector<string> getInfoText(string filename);

 private:
  // Private constructor ensures class is not instantiated
  LevelParser ();

  // File stream
  static std::ifstream file;
  // Character that indicates the line is commented
  static char commentChar;

};

#endif
