/** Pulser.cpp
 *  The pulser is a security bit that detects anomalies within its range.
 *  It pulses in a set sequence.
 */

#include "Pulser.h"
#include "PulseEvent.h"
#include "Animation.h"

using namespace std;

/** CONSTRUCTORS **/
Pulser::Pulser (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  maxResistance = resistance = 4;
  vector<string> imgs;
  //sets the images
  imgs.push_back( "new_pulser_bit.png" );
  imgs.push_back( "new_pulser_center.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs, PULSER );
  }
  pulseRadius = 2;
}

/** UTILITY FUNCTIONS **/
void Pulser::queueStandardActionOrders () {
  for (int i = 0; i < standardActionOrders.size(); ++i ) {
    actionQueue.push_back( standardActionOrders.at(i) );
  }
}

//pulse events are created here
void Pulser::downCycle () {

  //cycle through standard action orders
  if( actionQueue.empty() ) return;
  else if( actionQueue.front() == 1 ) {

    vector<Location> locationsToPulse;
    vector<Location> myLocs = getLocations();
    Location tempLoc;
    //adds all the Locations within pulseRadius to vector 
    for(int i = -pulseRadius; i <= pulseRadius; ++i){
      for(int j = 0; j <= pulseRadius - abs(i); ++j)
	{
	  tempLoc.x = myLocs[0].x + i;
	  tempLoc.y = myLocs[0].y + j;
	  locationsToPulse.push_back(tempLoc);
	  if(j){ //adds negatives but not 0 again
	    tempLoc.x = myLocs[0].x + i;
	    tempLoc.y = myLocs[0].y - j;
	    locationsToPulse.push_back(tempLoc);
	  }
	}
    } 
    
    PulseEvent ev( this, locationsToPulse );
    ev.commit( controlGroup->level );
  }
  
  actionQueue.pop_front();
  if( actionQueue.empty() ) queueStandardActionOrders();
  
}

/** ACCESSORS **/
int Pulser::getRadius() {
  return pulseRadius;
}

/** MUTATORS **/
void Pulser::setRadius(int radius) {
  pulseRadius = radius;
}
 
 //sets the orders ie pulsing or waiting etc
void Pulser::setStandardActionOrders ( std::vector<int> s ) {
  standardActionOrders = s;
  queueStandardActionOrders();
}
