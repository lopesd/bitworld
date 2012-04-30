#include "ZapEvent.h"
#include "Sentinel.h"

using namespace std;

ZapEvent::ZapEvent ( void* sender, Location l ) 
  : Event( sender ) {
  sentinel = (Sentinel*)sender;
  loc = l;
}

void ZapEvent::commit ( Level* level ) {


	CellGroup* zappedUnit = level->unitAtLocation( loc );
	if(zappedUnit)
		if(sentinel->controlGroup != zappedUnit->controlGroup)
			level->flagUnit(zappedUnit);
}
