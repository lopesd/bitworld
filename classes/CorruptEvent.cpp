/** CorruptEvent.cpp
 *  The corrupt event is a kind of event instantiated by the Viral Bit that handles corruption logic.
 *  Also, possibly animation.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "CorruptEvent.h"
#include "ViralBit.h"

using namespace std;

/** CONSTRUCTORS **/
CorruptEvent::CorruptEvent ( void* sender, vector<CellGroup*> u ) 
  : Event( sender ) {
  viralBit = (ViralBit*)sender;
  unitsToCorrupt = u;
}

/** UTILITY FUNCTIONS **/
void CorruptEvent::commit ( Level* level ) {

  for( int k = 0; k < unitsToCorrupt.size(); ++k ) {
    unitsToCorrupt[k]->controlGroup->forfeit( unitsToCorrupt[k] );
    viralBit->controlGroup->take( unitsToCorrupt[k] );
  }

}
