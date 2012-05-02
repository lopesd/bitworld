/*  ZapEvent.cpp
 *  Commits an event by flagging a bit at Location loc if an enemy bit exists there. Created by
 *  a Sentinel.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "ZapEvent.h"
#include "Sentinel.h"

using namespace std;

/**CONSTRUCTOR **/
ZapEvent::ZapEvent ( void* sender, Location l ) 
  : Event( sender ) {
  sentinel = (Sentinel*)sender;
  loc = l;
}

void ZapEvent::commit ( Level* level ) { //commits to level
  CellGroup* zappedUnit = level->unitAtLocation( loc );
  if(zappedUnit) //if there is a unit at the location
    if(sentinel->controlGroup != zappedUnit->controlGroup) //if doesn't belong to your control group flag the bit
			level->flagUnit(zappedUnit);
}

