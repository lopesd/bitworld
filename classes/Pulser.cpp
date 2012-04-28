/** Pulser.cpp
 *  The pulser is a security bit that detects anomalies within its range.
 *  It pulses in a set sequence.
 */

#include "Pulser.h"
#include "PulseEvent.h"
#include "Animation.h"

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
    vector<Location> locationsToPulse;
    vector<Location> myLocs = getLocations();
    Location tempLoc;
    for(int i = -pulseRadius; i <= pulseRadius; ++i)
      {
	for(int j = 0; j <= pulseRadius - abs(i); ++j)
	  {
	    tempLoc.x = myLocs[0].x + i;
	    tempLoc.y = myLocs[0].y + j;
	    locationsToPulse.push_back(tempLoc);
	    if(j)
	      {
		tempLoc.x = myLocs[0].x + i;
		tempLoc.y = myLocs[0].y - j;
		locationsToPulse.push_back(tempLoc);
	      }
	  }
      } 
    
    // Create circle pulsing animation
    Animation anim( getLocations()[0] );
    anim.addImage( "pulse_radius.png" );
    anim.addImage( "pulse_glow.png" );
    anim.addImage( "pulse_radius.png" );
    anim.addImage( "pulse_glow.png" );
    anim.setSizeInterval( 0.001, pulseRadius*2 + 1 );
    anim.setAlphaInterval( 255, 0 );
    anim.commit( controlGroup->level );
    
    // Create glowing animation
    vector<float> alphas( 3, 0 );
    alphas[1] = 70;
    for( int i = 0; i < locationsToPulse.size(); ++i ) {
      Animation glow( locationsToPulse[i] );
      glow.addImage( "pulse_glow.png" );
      glow.setAlphaInterval( alphas );
      glow.commit( controlGroup->level );
    }

    PulseEvent ev( this, locationsToPulse );
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
