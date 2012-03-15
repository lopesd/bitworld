#include "LevelParser.h"
#include "Bit.h"
#include "Pulser.h"
#include "UserControlGroup.h"
#include "AIControlGroup.h"
#include "Level.h"

using namespace std;

LevelParser::LevelParser () {
  commentChar = '#';
}

LevelParser::~LevelParser () {
  
}

Level LevelParser::parse (const char* filename, sf::RenderWindow& window) {
  enum Macro {METADATA, GRID, EVENT};
  enum Macro macro = GRID;
  string token;

  map <string, map<char, Cell> > cellsMap;
  map <string, CellGroup*> unitsMap;
  CellGroup* unit;
  
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
	cout << endl << "Setting macro" << endl;
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
	  if ( token[0] == 'B' || token[0] == 'G' ) { // Have different if statements for different kinds of cells, if needed
	    Cell cell (gridX, gridY);
	    cout << "Creating new " << token[0] << " cell at " << gridX << " " << gridY << endl;
	    cellsMap[ token.substr(0,token.length()-1) ][ token[token.length()-1] ] = cell; //inserts a cell in the map
	    cout << "Cell mapped to " << token.substr(0,token.length()-1) << ", piece " << token[token.length()-1] << endl;
	  }
	  
	  // INVALID UNIT TYPE, SKIP TOKEN
	  else {
	    cout << "Skipping token " << token << endl;
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
	       << "Unit " << token << " will be instantiated with an empty cell vector. " << endl;
	}
	
	vector<Cell> cellVector;
	map<char,Cell> tempMap = cellsMap[ token.substr(0,token.length()-1) ];
	cout << "tempMap size is " << tempMap.size() << endl;
	for ( map<char, Cell>::iterator it = tempMap.begin(); it != tempMap.end(); ++it ) {
	  cout << "adding a cell to the cellGroup..." << endl;
	  cellVector.push_back( it->second );
	}

	// CHECK TO SEE WHICH TYPE IT NEEDS TO BE, THEN CREATE AND GET POINTER
	if ( token[0] == 'B' ) {  // BIT
	  cout << "Creating new Bit" << endl;
	  unit = new Bit (cellVector);
	} 
	else if ( token[0] == 'G' ) { // GATE, PULSER FOR NOW
	  cout << "Creating new pulser (from token G)" << endl;
	  unit = new Pulser (cellVector);
	}
	
	// INVALID UNIT TYPE, SKIP LINE
	else {
	  cout << "Warning -- invalid unit type " << token << "; killing line" << endl;
	  getline (file, token);
	  continue;
	}
	
	unitsMap.insert ( pair<string, CellGroup*> ( token.substr(0,token.length()-1), unit ) ); // may be unnecessary for now, but will probably be necessary for events
	
	// MODIFY ATTRIBUTES OF CELL WITH NEXT LINES
	do {
	  file >> token;
	  cout << token;
	  //do stuff here, do not assume line does not end in ';'
	}
	while ( token[token.length()-1] != ';' ); //while the line is not over
      }
      
      // EVENT: create event objects?
      
    }
  }

  else
    cout << "Failure to open level file " << filename << endl;
  file.close();
  
  vector<CellGroup*> unitsVector;
  for ( map<string, CellGroup*>::iterator it = unitsMap.begin(); it != unitsMap.end(); ++it )
    unitsVector.push_back( it->second );
    
  UserControlGroup* user = new UserControlGroup (unitsVector); //For now, all units are being placed under user control
  //initialize other groups
  
  vector<ControlGroup*> controlGroups;
  controlGroups.push_back( user );

  Level level (window, controlGroups, unitsVector, gridX, gridY);
  cout << "width: " << gridX << ", height: " << gridY << endl;
  return level;
}
