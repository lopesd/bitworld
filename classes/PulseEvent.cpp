#include "PulseEvent.h"
#include "Pulser.h"

using namespace std;

PulseEvent::PulseEvent ( void* sender, vector<Location> l ) 
  : Event( sender ) {
  pulser = (Pulser*)sender;
  locs = l;
}

void PulseEvent::commit ( Level* level ) {

  for(int j = 0; j < locs.size(); j++) {
    CellGroup* pulsedUnit = level->unitAtLocation( locs[j] );
    //Animation p( PULSE, locs[j] );
    //p.commit( level );
    if( pulsedUnit ) {
      if( pulser->controlGroup != pulsedUnit->controlGroup ) { //If the unit pulsed is an enemy
	level->flagUnit( pulsedUnit );
	//Animation d( DETECT, locs[j] );
	//d.commit( level );
      }
    }
  }
  
}
