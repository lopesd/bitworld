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
  imgs.push_back( "new_pulser_bit.png" );
  imgs.push_back( "new_pulser_center.png" );
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
    
    makeAnimation( locationsToPulse );
    PulseEvent ev( this, locationsToPulse );
    ev.commit( controlGroup->level );
  }
  
  actionQueue.pop_front();
  if( actionQueue.empty() ) queueStandardActionOrders();

}

void Pulser::makeAnimation ( vector<Location> locationsToPulse ) {

    // Create glowing animations
    vector<float> alphas( 3, 0 );
    alphas[1] = 130;
    for( int i = 0; i < locationsToPulse.size(); ++i ) {
      Animation glow( locationsToPulse[i] );
      glow.addImage( "pulse_glow.png" );
      glow.setAlphaInterval( alphas );
      glow.commit( controlGroup->level );
    }
    
    // Create circle pulsing animation
    Animation pulse( getLocations()[0] );
    vector<float> sizes( 3, 1 );
    sizes[0] = 0.001;
    sizes[1] = 3*(pulseRadius*2)/4;
    sizes[2] = pulseRadius*2;
    pulse.addImage( "pulse_radius.png" );
    pulse.setSizeInterval( sizes );
    pulse.setAlphaInterval( 255, 0 );
    
    pulse.commit( controlGroup->level );

    /*
    // For the lulz
    Animation effect = pulse;
    sizes[1] = 40;
    sizes[2] = 55;
    effect.setAlphaInterval( 50, 0 );
    effect.setSizeInterval( sizes );

    pulse.commit  ( controlGroup->level );
    effect.commit ( controlGroup->level );

    // For the lulz (2)
    Animation radar( getLocations()[0] );
    radar.addImage( "pulse_radar.png" );
    radar.setSizeInterval( pulseRadius*2+1, pulseRadius*2+1 );
    radar.setRotationInterval( 0, 360 );
    radar.setAlphaInterval( 140, 0 );
    radar.commit( controlGroup->level );
    */
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
