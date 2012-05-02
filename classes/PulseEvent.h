/** PulseEvent.h
 *  A pulse event is instantiated by the pulser and commited to a level.
 *  It handles the pulsing logic (detecting units) as well as animation.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef PULSE_EVENT_H
#define PULSE_EVENT_H

#include "Event.h"

class Pulser; //forward declare viral bit

class PulseEvent : public Event {
  
 public:
  /** CONSTRUCTORS **/
  PulseEvent( void* sender, std::vector<Location> locs );

  /** UTILITY FUNCTIONS **/
  // Commit to a given level
  void commit( Level* level );

 private:
  void makePulseAnimation ( Level* level ); // Create the generic purple pulse animation
  void makeDetectAnimation ( Location loc, Level* level ); // Create the white flash the signals a detection
  Pulser* pulser; // Pointer to the sender, cast as a pulser
  std::vector<Location> locs; // Vector of locations that need to be pulsed

};

#endif
