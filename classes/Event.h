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
  Event( std::vector<Location> locs, EventType type, void* sender );
  Event( EventType t = EMPTY );
  Event( EventType t, void* sender );
  Event( const Event& );

  void send( Level* receiver );
  
  EventType type;
  void* sender;
  
 private:
  std::vector<Location> locations;

  int ID; // for debugging. each event has a unique ID number
  static int IDCount;

};

#endif
