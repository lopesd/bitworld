/** Event.cpp
 *  An event object defines things that "happen" on the level. Speciliazed events, such as a pulse event
 *  or a corrupt event, inherit from this base class. 
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "Event.h"

using namespace std;

/** CONSTRUCTORS **/
Event::Event( void* s ) {
  sender = sender;
}

/** UTILITY FUNCTIONS **/
void Event::commit ( Level* level ) {}
