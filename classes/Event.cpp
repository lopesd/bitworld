#include "Event.h"

using namespace std;

int Event::IDCount = 0;

Event::Event( EventType t, void* s ) {
  type = t;
  sender = s;
  ID = IDCount++;  
}

Event::Event( EventType t, void* s, std::vector<Location> locs ) {
  locations = locs;
  type = t;
  sender = s;
  ID = IDCount++;  
}

Event::Event( EventType t, void* s, std::vector<CellGroup*> u ) {
  units = u;
  type = t;
  sender = s;
  ID = IDCount++;
}

Event::Event( const Event& E ) {
  ID = IDCount++;
  sender = E.sender;
  locations = E.locations;
  units = E.units;
  type = E.type;
}

