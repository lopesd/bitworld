#include "Pulser.h"

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

void Pulser::queueStandardActionOrders (int cycles) {
  for (int i = 0; i < cycles; ++i, ++SAOCounter) {
    actionQueue.push_back( standardActionOrders.at( SAOCounter ));
    if (SAOCounter >= standardActionOrders.size())
      SAOCounter = 0;
  }
}

void Pulser::setRadius(int radius) {
	pulseRadius = radius;
}


int Pulser::getRadius()
{
	return pulseRadius;
}

vector<Event> Pulser::downCycle () {
  
  EventType t = PULSE;	
  vector<Location> temp;
  vector<Location> myLocs = getLocations();
  Location tempLoc;
  for(int i = -pulseRadius; i <= pulseRadius; ++i)
    {
      for(int j = 0; j <= pulseRadius - abs(i); ++j)
	{
	  tempLoc.x = myLocs[0].x + i;
	  tempLoc.y = myLocs[0].y +j;
	  temp.push_back(tempLoc);
	  if(j)
	    {
	      tempLoc.x = myLocs[0].x + i;
	      tempLoc.y = myLocs[0].y - j;
	      temp.push_back(tempLoc);
	    }
	}
    } 
  
  vector<Event> evs;
  evs.push_back( Event(t, this, temp) );
  return evs;
}
