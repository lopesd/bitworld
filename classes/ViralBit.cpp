/*  ViralBit.cpp
 *  The starting user bit. It can corrupt other bits by staying adjacent to them
 *  for a certain number of cycles depending on the unit's resistance.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */
#include "ViralBit.h"
#include "CorruptEvent.h"

using namespace std;

ViralBit::ViralBit (vector<Cell> c) : CellGroup (c) { 
  weight = 2;
  vector<string> imgs;
  imgs.push_back( "new_viral_bit.png" );
  imgs.push_back( "new_viral_bit2.png" );
  for (int i = 0; i < cells.size(); ++i) { //sets the images for the viral bit
    cells[i].setImages( imgs );
  }
}

//Where the corruption happens
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
      if( CellGroup* unit = controlGroup->level->unitAtLocation(temp[j]) ) // if there is an enemy unit, drop resitance until 0 and then corrupt it
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
