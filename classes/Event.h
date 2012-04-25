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
  Event( void* sender );

  virtual void commit ( Level* level );

  void* sender;
  
 private:
  int ID; // for debugging. each event has a unique ID number
  static int IDCount;

};

#endif
