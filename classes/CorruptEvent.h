#ifndef CORRUPT_EVENT_H
#define CORRUPT_EVENT_H

#include "Event.h"

class ViralBit; //forward declare viral bit

class CorruptEvent : public Event {
  
 public:
  CorruptEvent( void* sender, std::vector<CellGroup*> unitsToCorrupt );

  void commit( Level* level = 0 );

 private:
  ViralBit* viralBit;
  std::vector<CellGroup*> unitsToCorrupt;

};

#endif
