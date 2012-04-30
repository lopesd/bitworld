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
  imgs.push_back( "new_pulser_bit.png" );
  imgs.push_back( "pulser_center.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs, PULSER );
  }
  
}

void Sentinel::queueStandardActionOrders () {
  for (int i = 0; i < standardActionOrders.size(); ++i ) {
    actionQueue.push_back( standardActionOrders.at(i) );
  }
}

void Sentinel::downCycle () {

  //cycle through standard action orders
  if( actionQueue.empty() ) return;
  else if( actionQueue.front() == 1 ) {

    vector<Location> locationsToZap;
    Location tempLoc = getLocations()[0] +dir;
    while(!(controlGroup->level->unitAtLocation(tempLoc))){
    	if(tempLoc.x > -1 && tempLoc.x < controlGroup->level->getWidth() && tempLoc.y  > -1 && tempLoc.y < controlGroup->level->getHeight()){
    		locationsToZap.push_back(tempLoc);
    		tempLoc = tempLoc + dir;
    	}
    	else
    		break;
    }
    if(controlGroup->level->unitAtLocation(tempLoc))
    {
    	if(controlGroup->level->unitAtLocation(tempLoc)->controlGroup != controlGroup)
    	{
    		locationToZap = tempLoc;
    		//locationsToZap.push_back(tempLoc);
    	}
    }
    
    ZapEvent ev( this, locationToZap);
    ev.commit( controlGroup->level );
    
    makeAnimation( locationsToZap);
  }
  
  actionQueue.pop_front();
  if( actionQueue.empty() ) queueStandardActionOrders();

}

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
    /*
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
		*/
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

void Sentinel::setDirection(Direction d){
	dir = d;
}

Direction Sentinel::getDirection(){
 return dir;
}
void Sentinel::setStandardActionOrders ( std::vector<int> s ) {
  standardActionOrders = s;
  queueStandardActionOrders();
}
