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
  Event( EventType type = EMPTY, void* sender = 0 );
  Event( EventType type, void* sender, std::vector<Location> locations );
  Event( EventType type, void* sender, std::vector<CellGroup*> units );
  
  Event( const Event& );

  EventType type;
  void* sender;
  std::vector<Location> locations;
  std::vector<CellGroup*> units;
  
 private:
  int ID; // for debugging. each event has a unique ID number
  static int IDCount;

};

#endif
