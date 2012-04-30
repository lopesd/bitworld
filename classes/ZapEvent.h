#ifndef ZAP_EVENT_H
#define ZAP_EVENT_H

#include "Event.h"

class Sentinel; //forward declare viral bit

class ZapEvent : public Event {
  
 public:
  ZapEvent( void* sender, Location loc );

  void commit( Level* level );

 private:
  Sentinel* sentinel;
  Location loc;

};

#endif
