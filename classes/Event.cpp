#include "Event.h"

using namespace std;

int Event::IDCount = 0;

Event::Event( vector<Location> locs,  EventType t, void* s ) {
  locations = locs;
  type = t;
  sender = s;
  ID = IDCount++;
}

Event::Event( EventType t ) {
  type = t;
  sender = 0;
  ID = IDCount++;
}

Event::Event( EventType t, void* s ) {
  type = t;
  sender = s;
  ID = IDCount++;
}

Event::Event( const Event& E ) {
  ID = IDCount++;
  sender = E.sender;
  locations = E.locations;
  type = E.type;
}

void Event::send( Level* receiver ) {
  
}
