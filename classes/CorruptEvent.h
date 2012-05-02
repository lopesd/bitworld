/** CorruptEvent.h
 *  The corrupt event is a kind of event instantiated by the Viral Bit that handles corruption logic.
 *  Also, possibly animation.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef CORRUPT_EVENT_H
#define CORRUPT_EVENT_H

#include "Event.h"

class ViralBit; //forward declare viral bit

class CorruptEvent : public Event {

 public:
  /** CONSTRUCTORS **/
  CorruptEvent( void* sender, std::vector<CellGroup*> unitsToCorrupt );

  /** UTILITY FUNCTIONS **/
  // Commit the effects of the event (the level doesn't matter, so we give it a default value of 0)
  void commit( Level* level = 0 );

 private:
  // Pointer to the viral bit that instantiated it. 
  ViralBit* viralBit;
  std::vector<CellGroup*> unitsToCorrupt;

};

#endif
