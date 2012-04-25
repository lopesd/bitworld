#include "CorruptEvent.h"
#include "ViralBit.h"

using namespace std;

CorruptEvent::CorruptEvent ( void* sender, vector<CellGroup*> u ) 
  : Event( sender ) {
  viralBit = (ViralBit*)sender;
  unitsToCorrupt = u;
}

void CorruptEvent::commit ( Level* level ) {

  for( int k = 0; k < unitsToCorrupt.size(); ++k ) {
    unitsToCorrupt[k]->controlGroup->forfeit( unitsToCorrupt[k] );
    viralBit->controlGroup->take( unitsToCorrupt[k] );
  }

}
