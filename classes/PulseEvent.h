/*  PulseEvent.h
 *  Commits an event by flagging a bit at Location loc if an enemy bit exists there. Created by
 *  a Pulser.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef PULSE_EVENT_H
#define PULSE_EVENT_H

#include "Event.h"

class Pulser; //forward declare viral bit

class PulseEvent : public Event {
  
 public:
 	/** CONSTRUCTOR **/
  PulseEvent( void* sender, std::vector<Location> locs );

	/** UTILITY **/
	
	//commits it to Level
  void commit( Level* level );

 private:
  Pulser* pulser;
  std::vector<Location> locs; //locations to pulse

};

#endif
