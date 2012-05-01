/*  ZapEvent.h
 *  Commits an event by flagging a bit at Location loc if an enemy bit exists there. Created by
 *  a Sentinel.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef ZAP_EVENT_H
#define ZAP_EVENT_H

#include "Event.h"

class Sentinel; //forward declare Sentinel bit

class ZapEvent : public Event {
  
 public:
 	/**CONSTRUCTOR **/
  ZapEvent( void* sender, Location loc ); 

  void commit( Level* level ); //tells level to do it

 private:
  Sentinel* sentinel;
  Location loc;

};

#endif
