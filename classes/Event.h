/** Event.h
 *  An event object defines things that "happen" on the level. Speciliazed events, such as a pulse event
 *  or a corrupt event, inherit from this base class. 
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef EVENT_H
#define EVENT_H

#include "structures.h"
#include "CellGroup.h"
#include "Level.h"
#include "Gate.h"
#include <vector>

class Level;
class CellGroup; //Forward declarations
class Gate;

class Event {

 public:
  /** CONTSTRUCTORS **/
  Event( void* sender );

  /** UTILITY FUNCTIONS **/
  // Commit the changes of the event to the given level
  virtual void commit ( Level* level );

  void* sender;
  
 private:

};

#endif
