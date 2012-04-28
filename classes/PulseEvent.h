#ifndef PULSE_EVENT_H
#define PULSE_EVENT_H

#include "Event.h"

class Pulser; //forward declare viral bit

class PulseEvent : public Event {
  
 public:
  PulseEvent( void* sender, std::vector<Location> locs );

  void commit( Level* level );

 private:
  Pulser* pulser;
  std::vector<Location> locs;

};

#endif
