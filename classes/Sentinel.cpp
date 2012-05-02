/** Sentinel.cpp
 *  The sentinel is a security bit that detects anomalies in a direct line in front of it.
 *  It pulses/zaps/scans/whatevers in a set sequence.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "Sentinel.h"
#include "ZapEvent.h"
#include "Animation.h"

using namespace std;

Sentinel::Sentinel (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  maxResistance = resistance = 5;
  //setting images
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "sentinel1.png" );
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
		 ZapEvent ev( this, locationToZap); //zap at locationToZaP
   	 ev.commit( controlGroup->level );
	  }
      }
    

    
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
      trace.addImage( "sentinelZap2.png" );
      trace.setAlphaInterval( alphas );
      trace.setRotationInterval( rotation, rotation );
      trace.commit( controlGroup->level );
    }

}

Direction Sentinel::getDirection () {
 return dir;
}

// Set the sentinel's direction
void Sentinel::setDirection ( Direction d ){
  dir = d;  
  rotation = (dir.y*90+90) + (int)(dir.x != 0)*(dir.x*90+90); // calculate the rotation based on direction

  for( int i = 0; i < cells.size(); ++i )
    cells[i].setSpriteRotation( rotation );
}

void Sentinel::setStandardActionOrders ( std::vector<int> s ) {
  standardActionOrders = s;
  queueStandardActionOrders();
}
