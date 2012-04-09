/** LevelParser.cpp
 *  LevelParser is a static class. It reads texts files and returns constructed Level objects.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "LevelParser.h"
#include "Bit.h"
#include "Pulser.h"
#include "ViralBit.h"
#include "WhiteBit.h"
#include "Gate.h"
#include "UserControlGroup.h"
#include "AIControlGroup.h"
#include "Level.h"

#include <vector>
#include <map>
#include <cstring>

using namespace std;

// Initialize static variables
ifstream LevelParser::file;
char LevelParser::commentChar = '#';

// Parse text file into level object
Level LevelParser::Parse ( const char* filename, sf::RenderWindow& window ) {
  enum Macro {METADATA, GRID, EVENT};
  enum Macro macro = GRID;
  string defaultCg = "AI";
  string token;

  map <string, map<char, Cell> > cellsMap;
  map <string, CellGroup*>       unitsMap;
  map <string, Gate*>            gatesMap;
  CellGroup* unit;
  Gate*      gate;
  
  int gridX = 0, gridY = 0; // Keep track of the location on the grid

  file.open (filename);

  if (file.is_open()) {
    while (file.good()) {

      file >> token; //read the first token

      // CHECK FOR COMMENTS, BREAK IF ONE IS FOUND
      if (token[0] == commentChar) {
	getline(file,token); // kill the line
	continue;
      }
      
      // Check and set the macro if it is being changed
      if (token[0] == '~') { 
	if (token == "~METADATA")    macro = METADATA;
	else if (token == "~GRID")   macro = GRID;
	else if (token == "~EVENT")  macro = EVENT; 
	continue;
      }

      // GRID: create cell objects
      if (macro == GRID) {

	gridX = 0;
	do {	  
	  // CHECK TO SEE WHICH TYPE IT NEEDS TO BE, THEN CREATE AND GET POINTER
	  // Have different if statements for different kinds of cells, if needed
	  if ( token[0] == 'B' || token[0] == 'G' || token[0] == 'P' || token[0] == 'V' || token[0] == 'K' ) { 
	    Cell cell (gridX, gridY);
	    //inserts a cell in the map
	    cellsMap[ token.substr(0,token.length()-1) ][ token[token.length()-1] ] = cell; 
	  }
	  
	  // INVALID UNIT TYPE, SKIP TOKEN
	  else {
	    //cout << "Skipping token " << token << endl;
	  }

	  ++gridX;

	  file >> token;
	} while ( token[token.length()-1] != ';' );

	++gridY;   //increment the column counter
      }
      

      //METADATA: create unit objects
      else if (macro == METADATA) {
	
	// IF THERE ARE NO CELLS ASSOCIATED WITH THAT UNIT, THROW ERROR MESSAGE
	if ( cellsMap.find( token.substr(0,2) ) == cellsMap.end() ) {
	  cout << "WARNING: Unit description in metadata does not match any units in grid. "
	       << "\"" << token << "\" will be instantiated with an empty cell vector. " << endl;
	}
	
	vector<Cell> cellVector;
	map<char,Cell> tempMap = cellsMap[ token.substr(0,token.length()-1) ];
	//cout << "tempMap size is " << tempMap.size() << endl;
	for ( map<char, Cell>::iterator it = tempMap.begin(); it != tempMap.end(); ++it ) {
	  //cout << "adding a cell to the cellGroup..." << endl;
	  cellVector.push_back( it->second );
	}

	// CHECK TO SEE WHICH TYPE IT NEEDS TO BE, THEN CREATE AND GET POINTER
	if      ( token[0] == 'B' ) { // BIT, OR GATE
	  unit = new Bit (cellVector);
	  unitsMap[token.substr(0,token.length()-1)] = unit; // may be unnecessary for now, but will probably be necessary for events
	}
	else if ( token[0] == 'P' ) {// PULSER
	  unit = new Pulser (cellVector);
	  unitsMap[token.substr(0,token.length()-1)] = unit; // may be unnecessary for now, but will probably be necessary for events
	}
	else if ( token[0] == 'V' ) { // VIRAL BIT
	  unit = new ViralBit (cellVector);
	  unitsMap[token.substr(0,token.length()-1)] = unit; // may be unnecessary for now, but will probably be necessary for events
	}
	else if ( token[0] == 'K' ) { // VIRAL BIT
	  unit = new WhiteBit (cellVector);	  
	  unitsMap[token.substr(0,token.length()-1)] = unit; // may be unnecessary for now, but will probably be necessary for events
	}
	else if ( token[0] == 'G' ) {
	  gate = new Gate (cellVector);
	  gatesMap[token.substr(0,token.length()-1)] = gate; // may be unnecessary for now, but will probably be necessary for events
	}
	
	// INVALID UNIT TYPE, SKIP LINE
	else {
	  //cout << "Warning -- invalid unit type " << token << "; killing line" << endl;
	  getline (file, token);
	  continue;
	}
	
	// MODIFY ATTRIBUTES OF CELL WITH NEXT LINES
	do {
	  file >> token;

	  // HANDLE FLAGS
	  if( token[0] == '-' ) { 
	    
	    //-cg sets the next token to the CG name
	    if( strcmp(token.c_str(), "-cg") == 0 ) { 
	      file >> token;
	      unit->CGGroupName = token;
	    }
	    
	    //-move sets the following string of chars to the movement orders
	    else if( strcmp(token.c_str(), "-move") == 0 ) { 
	      file >> token;
	      vector<Direction> directions;
	      Direction temp;
	      cout << "Token size is " << token.size() << endl;
	      for( int i = 0; i < token.size(); ++i ) {
		switch (token[i]) {
		  case 'u':
		    temp.x = 0; temp.y = -1;
		    break;
		  case 'd':
		    temp.x = 0; temp.y = 1;
		    break;
		  case 'l':
		    temp.x = -1; temp.y = 0;
		    break;
		  case 'r':
		    temp.x = 1; temp.y = 0;
		    break;
		  case 's':
		    temp.x = 0; temp.y = 0;
		    break;
		  }
		directions.push_back( temp );
	      }
	      unit->setSMO( directions );
	    }

	    // -weight sets the weight of a gate
	    else if( strcmp(token.c_str(), "-weight") == 0 ) {
	      int newWeight;
	      file >> newWeight;
	      gate->setWeight( newWeight );
	    }

	    // -destination sets the destination of a gate
	    else if( strcmp(token.c_str(), "-destination") == 0 ) {
	      file >> token;
	      gate->setDest( token );
	    }
	    
	  } 
	  
	  // NON-FLAGS
	  else {
	    //do stuff here, do not assume line does not end in ';'	  
	  }

	}
	while ( token[token.length()-1] != ';' ); //while the line is not over
      }
      
      // EVENT: create event objects?
      
    }
  }

  else
    cout << "Failure to open level file " << filename << endl;
  file.close();
  
  // PUT IT ALL TOGETHER
  // This vector contains pointers to all the units
  vector<CellGroup*> userUnitsVector;
  vector<CellGroup*> AIUnitsVector;
  vector<CellGroup*> unitsVector;
  vector<Gate*>      gatesVector;
  for( map<string, CellGroup*>::iterator it = unitsMap.begin(); it != unitsMap.end(); ++it ) {
    unitsVector.push_back( it->second );
    if( strcmp(it->second->CGGroupName.c_str(), "user") == 0 )
      userUnitsVector.push_back( it->second );
    else
      AIUnitsVector.push_back( it->second );
  }
  for( map<string, Gate*>::iterator it = gatesMap.begin(); it != gatesMap.end(); ++it ) {
    gatesVector.push_back( it->second );
  }
  
  UserControlGroup* user = new UserControlGroup (userUnitsVector);
  AIControlGroup*   AI   = new AIControlGroup   (AIUnitsVector);
  
  vector<ControlGroup*> controlGroups;
  controlGroups.push_back( AI );
  controlGroups.push_back( user );

  Level level (window, controlGroups, unitsVector, gatesVector, gridX, gridY);

  cout << "Returning level!" << endl;
  return level;
}
