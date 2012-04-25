#include "Event.h"

using namespace std;

int Event::IDCount = 0;

Event::Event( void* s ) {
  sender = sender;
  ID = IDCount++;
}


void Event::commit ( Level* level ) {
  
}
