#include "Event.h"

using namespace std;

Event::Event( vector<Location> locs, enum Type t, CellGroup* s ) {
  locations = locs;
  type = t;
  sender = s;
}

void Event::send( Level* receiver ) {
  
}
