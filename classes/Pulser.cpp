/** Pulser.cpp
 *  The pulser is a security bit that detects anomalies within its range.
 *  It pulses in a set sequence.
 */

#include "Pulser.h"
#include "PulseEvent.h"

using namespace std;

Pulser::Pulser (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  maxResistance = resistance = 5;
  vector<string> imgs;
  imgs.push_back( "pulser_bit2.png" );
  imgs.push_back( "pulser_center.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs, PULSER );
  }
  pulseRadius = 2;
}

void Pulser::queueStandardActionOrders () {
  for (int i = 0; i < standardActionOrders.size(); ++i ) {
    actionQueue.push_back( standardActionOrders.at(i) );
  }
}

void Pulser::downCycle () {

  //cycle through standard action orders
  if( actionQueue.empty() ) return;
  else if( actionQueue.front() == 1 ) {

    cout << "Pulsing!" << endl;
    vector<Location> temp;
    vector<Location> myLocs = getLocations();
    Location tempLoc;
    for(int i = -pulseRadius; i <= pulseRadius; ++i)
      {
	for(int j = 0; j <= pulseRadius - abs(i); ++j)
	  {
	    tempLoc.x = myLocs[0].x + i;
	    tempLoc.y = myLocs[0].y + j;
	    temp.push_back(tempLoc);
	    if(j)
	      {
		tempLoc.x = myLocs[0].x + i;
		tempLoc.y = myLocs[0].y - j;
		temp.push_back(tempLoc);
	      }
	  }
      } 
  
    PulseEvent ev( this, temp );
    ev.commit( controlGroup->level );
  }
  
  actionQueue.pop_front();
  if( actionQueue.empty() ) queueStandardActionOrders();

}

int Pulser::getRadius() {
  return pulseRadius;
}

void Pulser::setRadius(int radius) {
  pulseRadius = radius;
}
 
void Pulser::setStandardActionOrders ( std::vector<int> s ) {
  standardActionOrders = s;
  queueStandardActionOrders();
}
