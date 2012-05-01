/*  PulseEvent.cpp
 *  Commits an event by flagging a bit at Location loc if an enemy bit exists there. Created by
 *  a Pulser.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */
#include "PulseEvent.h"
#include "Pulser.h"

using namespace std;

/**CONSTRUCTOR **/
PulseEvent::PulseEvent ( void* sender, vector<Location> l ) 
  : Event( sender ) {
  pulser = (Pulser*)sender;
  locs = l;
}

void PulseEvent::commit ( Level* level ) { //commits it to level

  for(int j = 0; j < locs.size(); j++) {
    CellGroup* pulsedUnit = level->unitAtLocation( locs[j] );
    if( pulsedUnit ) {
      if( pulser->controlGroup != pulsedUnit->controlGroup ) { //If the unit pulsed is an enemy flag it
				level->flagUnit( pulsedUnit );
      }
    }
  }
  
}
