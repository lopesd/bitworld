/** CellGroup.cpp
 *  A CellGroup is a group of cells (usually one) that defines a single unit.
 *  It is responsible for drawing and moving its cells. It is the base class for specific kinds of units.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "CellGroup.h"

#include "Animation.h"

#include <iostream> // FOR TESTING

using namespace std;

const extern int FPS;

/** CONSTRUCTORS **/
CellGroup::CellGroup ( vector<Cell> c ) {
  freeToMove = 1;
  SMOCounter = 0;
  cells = c;
  controlGroup = 0;
  maxResistance = 1;
  resistance = maxResistance;
  resistanceDropped = 0;
  pathHead = getMiddle();
  CGGroupName = "AI"; // Default ControlGroup, used for level parsing
  for (int i = 0; i < cells.size(); i++) {
    locations.push_back( cells.at(i).getGridLocation() );
    cells.at(i).setMovementAnimation( WALK );
  }
}

/** UTILITY FUNCTIONS **/
void CellGroup::draw ( sf::RenderWindow& screen ) {
  for (vector<Cell>::iterator i = cells.begin(); i != cells.end(); ++i)
    i->draw( screen );
}

void CellGroup::removeLastMoveOrder() {
  if( movementQueue.empty() )
    return;

  pathHead.x -= movementQueue.back().x;
  pathHead.y -= movementQueue.back().y;
  movementQueue.pop_back();
}

// The default neighbor handler -- do nothing
void CellGroup::handleNeighbors (vector<CellGroup*> neighbors) {}

void CellGroup::move (Direction dir) {
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].move( dir );
    //controlGroup->level->requestDeafFrames( cells[i].getMoveCount() );
    controlGroup->level->requestDeafFrames( FPS/3 ); // taking the liberty to hard code...
  }
  
  locations.clear();
  for (int i = 0; i < cells.size(); i++) //update the locations vector
    locations.push_back( cells.at(i).getGridLocation() );
}

// Queue standard movement orders, in case they exist (AI)
void CellGroup::queueStandardMovementOrders () {
  Direction dir;
  for( int i = 0; i < standardMovementOrders.size(); ++i ) {
    dir = standardMovementOrders[i];
    issueMovementOrder( dir );
  }
}

// Issue one individual order
void CellGroup::issueMovementOrder ( Direction dir ) {
  movementQueue.push_back(dir);
  pathHead.x += dir.x;
  pathHead.y += dir.y;
}

// MOVEMENT OCCURS ON THE UPCYCLE
void CellGroup::upCycle () {
  // If no queued orders...
  if( movementQueue.empty() && standardMovementOrders.empty() )
    return;
  // If new orders can be given...
  if( movementQueue.empty() && !standardMovementOrders.empty() )
    queueStandardMovementOrders();

  if ( freeToMove ) {
    move( movementQueue.front() );
  }
  else {
    pathHead.x -= movementQueue.front().x;
    pathHead.y -= movementQueue.front().y;
  }
  freeToMove = 1;

  movementQueue.pop_front();
  // Check to see if we have exhausted our movement orders...
  if( movementQueue.empty() ) 
    if( standardMovementOrders.size() > 0 ) //If the unit has standard orders to follow (e.g. is AI)
      queueStandardMovementOrders();        //queue them  

  resetResistance();
}

// events occur on the downcycle. should be overloaded for classes that have events.
void CellGroup::downCycle () {}

/** ACCESSORS **/
vector<Location> CellGroup::getLocations () {
  return locations;
}

vector<FloatPair> CellGroup::getScreenLocations () {
  vector<FloatPair> locs;
  for (int i = 0; i < cells.size(); ++i) {
    locs.push_back( cells[i].getScreenLocation() );
  }
  return locs;
}

// Get future desired locations for each cell
vector<Location> CellGroup::getLocationPathHeads () {
  vector<Location> locs = getLocations();
  for( int i = 0; i < movementQueue.size(); ++i ) 
    for( int j = 0; j < locs.size(); ++j )
      locs[j] = locs[j] + movementQueue[i];
  return locs;
}

// Get a specific movement in the queue
Direction CellGroup::getMovement( int num ) {
  if( movementQueue.size() == 0 ) {
    Direction dir = {0,0};
    return dir;
  }
  return movementQueue[num];
}

int CellGroup::getWeight () {
  return weight;
}

FloatPair CellGroup::getPathHead () {
  return pathHead;
}

// Returns middle of CellGroup (average of cells' grid positions)
FloatPair CellGroup::getMiddle () {
  FloatPair middle = {0,0};
  int count;

  for (count = 0; count < cells.size(); ++count) { // Add x and y values
    Location temp = cells[count].getGridLocation();
    middle.x += temp.x;
    middle.y += temp.y;
  }
  middle.x /= count; // Average them
  middle.y /= count;
  
  return middle;
}

int CellGroup::getResistance () {
  return resistance;
}

int CellGroup::numOfMovements() {
  return movementQueue.size();
}

/** MUTATORS **/
// Only use this function if the unit is 1 cell big
void CellGroup::setLocation ( Location newLoc ) {
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].setGridLocation( newLoc );
  }
  clearMovementQueue();
  locations.clear();
  for (int i = 0; i < cells.size(); i++) //update the locations vector
    locations.push_back( cells.at(i).getGridLocation() );
  pathHead = getMiddle();
}

void CellGroup::setSMO ( vector<Direction> m ) {
  standardMovementOrders = m;
  queueStandardMovementOrders();
}

void CellGroup::clearMovementQueue () {
  movementQueue.clear();
  pathHead = getMiddle();
}

void CellGroup::setGridData ( int w, int h, int t, int l ) {
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setGridData( w, h, t, l );
  }
  pathHead = getMiddle();
}

void CellGroup::setFreeToMove ( int f ) {
  freeToMove = f;
}

void CellGroup::dropResistance ( int n ) {
  resistance -= n;
  resistanceDropped = 1;
}

// Corrupt the unit -- we assume he is a bit. 
void CellGroup::corrupt () {
  
  Animation baseImage( getLocations()[0] );
  baseImage.addImage( "new_simple_bit.png" );
  baseImage.commit( controlGroup->level );
  Animation fadeIn( getLocations()[0] );
  fadeIn.addImage( "corruptedBit.png" );
  fadeIn.setAlphaInterval( 0, 255 );
  fadeIn.commit( controlGroup->level );
  
  vector<string> imgs;
  imgs.push_back( "corruptedBit.png" );
  imgs.push_back( "corruptedBit2.png" );
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].setImages( imgs, NORMAL );
  }

}

void CellGroup::resetResistance () {
  if( !resistanceDropped ) resistance = maxResistance;
  resistanceDropped = 0;
}

void CellGroup::setMaxResistance (int n) {
  maxResistance = resistance = n;
}
