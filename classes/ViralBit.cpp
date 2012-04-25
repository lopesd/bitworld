#include "ViralBit.h"
#include "CorruptEvent.h"

using namespace std;

ViralBit::ViralBit (vector<Cell> c) : CellGroup (c) { 
  weight = 2;
  vector<string> imgs;
  imgs.push_back( "viral_bit.png" );
  imgs.push_back( "viral_bit2.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs );
  }
}

void ViralBit::downCycle () {
  vector<Location> myLocs = getLocations();
  vector<CellGroup*> unitsToCorrupt; //vector of units that have been corrupted
  
  for( int i = 0; i < myLocs.size(); ++i ) { //For every cell
    
    //All adjacent locations
    Location temp[4] = { 
      {myLocs[i].x + 1, myLocs[i].y},
      {myLocs[i].x - 1, myLocs[i].y},
      {myLocs[i].x, myLocs[i].y + 1},
      {myLocs[i].x, myLocs[i].y - 1}
    };
    
    for( int j = 0; j < 4; ++j ) {
      if( CellGroup* unit = controlGroup->level->unitAtLocation(temp[j]) )
	if( unit->controlGroup != controlGroup ) {
	  unit->dropResistance();
	  if( unit->getResistance() <= 0 ) {
	    unitsToCorrupt.push_back( unit );
	  }
	}
    }
  }

  //also create animation events
  if( !unitsToCorrupt.empty() ) {
    CorruptEvent ev( this, unitsToCorrupt );
    ev.commit();
  }
}
