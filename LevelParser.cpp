#include "LevelParser.h"

using namespace std;

LevelParser::LevelParser () {
  commentChar = '#';
}

LevelParser::~LevelParser () {
  
}

// Fills a string with the next line in the text file; returns 0 if line is empty or commented out
int LevelParser::getNextLine (string line) {
  getline (file, line);

  if (line[0] == commentChar)
    return 0;
  if (line.length() <= 0)
    return 0;
  
  return 1;
}

void LevelParser::parse (const char* filename) {
  enum Macro = {METADATA, GRID, EVENT};
  enum Macro macro = METADATA;
  string line;

  file.open (filename);

  if (file.is_open()) {
    while (file.good()) {
      if (getNextLine (line)) {

	// Check and set the macro if it is being changed
	if (line[0] == '~') { 
	  if (line == "~METADATA")    macro = METADATA;
	  else if (line == "~GRID")   macro = GRID;
	  else if (line == "~EVENT")  macro = EVENT; 
	}
	
	
	// METADATA: create cellGroup objects
	// GRID: create gridCell objects
	// EVENT: create event objects?

      }
    }
  }

  // Linking step -- place cells in cellGroups
  
  file.close();
}
