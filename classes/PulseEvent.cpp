/** PulseEvent.cpp
 *  A pulse event is instantiated by the pulser and commited to a level.
 *  It handles the pulsing logic (detecting units) as well as animation.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "PulseEvent.h"
#include "Pulser.h"

using namespace std;

/** CONSTRUCTORS **/
PulseEvent::PulseEvent ( void* sender, vector<Location> l ) 
  : Event( sender ) {
  pulser = (Pulser*)sender;
  locs = l;
}

/** UTILITY FUNCTIONS **/
void PulseEvent::commit ( Level* level ) {

  makePulseAnimation( level ); // create the full pulsing animation
  for(int j = 0; j < locs.size(); j++) { // for every location...
    CellGroup* pulsedUnit = level->unitAtLocation( locs[j] );
    if( pulsedUnit ) { // If there is someone in my pulse radius
      if( pulser->controlGroup != pulsedUnit->controlGroup ) { 
	//If the unit pulsed is an enemy, flag it!
	level->flagUnit( pulsedUnit );
	makeDetectAnimation( locs[j], level ); // create a detection animation
      }
    }
  }
  
}

// Make necessary animation objects
void PulseEvent::makePulseAnimation ( Level* level ) {

    // Create glowing animations
    vector<float> alphas( 3, 0 );
    alphas[1] = 130; // Set the alpha values to interpolate between
    for( int i = 0; i < locs.size(); ++i ) { // create a purple glow for every pulsed location
      Animation glow( locs[i] );
      glow.addImage( "pulse_glow.png" );
      glow.setAlphaInterval( alphas );
      glow.commit( level );
    }
    
    // Create circle pulsing animation
    Animation pulse( pulser->getLocations()[0] );
    float animSize = pulser->getRadius()*2+1;
    vector<float> sizes( 3, 1 );
    sizes[0] = 0.001;
    sizes[1] = 3*(animSize)/4;
    sizes[2] = animSize;
    pulse.addImage( "pulse_radius.png" );
    pulse.setSizeInterval( sizes );
    pulse.setAlphaInterval( 255, 0 );
    
    pulse.commit( level );

    /*
    // For the lulz, this is a cool and completely over-the-top effect
    Animation effect = pulse;
    sizes[1] = 40;
    sizes[2] = 55;
    effect.setAlphaInterval( 50, 0 );
    effect.setSizeInterval( sizes );

    pulse.commit  ( controlGroup->level );
    effect.commit ( controlGroup->level );
    */

}

// Create white flash animation when something has been detected
void PulseEvent::makeDetectAnimation ( Location loc, Level* level ) {
  Animation detect( loc );
  vector<float> alphas( 5, 0 );
  alphas[2] = 130;
  detect.setAlphaInterval( alphas );
  detect.setColor( sf::Color( 255, 255, 255, 255 ) ); // Set to full white
  detect.commit( level );
}
