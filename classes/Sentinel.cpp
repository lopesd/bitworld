/** Pulser.cpp
 *  The pulser is a security bit that detects anomalies within its range.
 *  It pulses in a set sequence.
 */

#include "Sentinel.h"
#include "ZapEvent.h"
#include "Animation.h"

using namespace std;

Sentinel::Sentinel (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  maxResistance = resistance = 5;
  vector<string> imgs;
  //setting images
  imgs.push_back( "new_pulser_bit.png" );
  imgs.push_back( "pulser_center.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs, PULSER );
  }
  
}

//add orders 
void Sentinel::queueStandardActionOrders () {
  for (int i = 0; i < standardActionOrders.size(); ++i ) {
    actionQueue.push_back( standardActionOrders.at(i) );
  }
}

//This is where zapEvents are created
void Sentinel::downCycle () {

  //cycle through standard action orders
  if( actionQueue.empty() ) return;
  else if( actionQueue.front() == 1 ) {

    vector<Location> locationsToZap;
    Location tempLoc = getLocations()[0] +dir;
    while(!(controlGroup->level->unitAtLocation(tempLoc))){ //if next location in direction doesn't have a unit
    	if(tempLoc.x > -1 && tempLoc.x < controlGroup->level->getWidth() && tempLoc.y  > -1 && tempLoc.y < controlGroup->level->getHeight()){ //if its in the grid
    		locationsToZap.push_back(tempLoc); //add to vector
    		tempLoc = tempLoc + dir;
    	}
    	else
    		break;
    }
    if(controlGroup->level->unitAtLocation(tempLoc)) 
    {
    	if(controlGroup->level->unitAtLocation(tempLoc)->controlGroup != controlGroup) //if unit at end is of different control group save loc for event
    	{
    		locationToZap = tempLoc; 
    	}
    }
    
    ZapEvent ev( this, locationToZap); //zap at locationToZaP
    ev.commit( controlGroup->level );
    
    makeAnimation( locationsToZap); //animation for tracer of zap
  }
  
  actionQueue.pop_front();
  if( actionQueue.empty() ) queueStandardActionOrders();

}

//animates tracer
void Sentinel::makeAnimation ( vector<Location> locationsToZap) {

    // Create glowing animations
    vector<float> alphas( 3, 0 );
    alphas[1] = 130;
    for( int i = 0; i < locationsToZap.size(); ++i ) {
      Animation trace( locationsToZap[i] );
      trace.addImage( "zap_trace.png" );
      trace.setAlphaInterval( alphas );
      trace.commit( controlGroup->level );
    }

}

Direction Sentinel::getDirection(){
 return dir;
}

void Sentinel::setDirection(Direction d){
	dir = d;
}

void Sentinel::setStandardActionOrders ( std::vector<int> s ) {
  standardActionOrders = s;
  queueStandardActionOrders();
}
