#include "CellGroup.h"

#include <iostream> // FOR TESTING

using namespace std;

CellGroup::CellGroup ( vector<Cell> c ) {
  freeToMove = 1;
  SMOCounter = 0;
  cells = c;
  controlGroup = 0;
  pathHead = getMiddle();
  CGGroupName = "AI"; // Default ControlGroup, used for level parsing
}

CellGroup::CellGroup ( Cell c ) {
  freeToMove = 1;
  SMOCounter = 0;
  cells.push_back( c );
  controlGroup = 0;
  pathHead = getMiddle();
  CGGroupName = "AI"; // Default ControlGroup, used for level parsing
}

CellGroup::CellGroup () {
  SMOCounter = 0;
}

CellGroup::~CellGroup () {

}

void CellGroup::draw ( sf::RenderWindow& screen ) {
  for (vector<Cell>::iterator i = cells.begin(); i != cells.end(); ++i)
    i->draw( screen );
}

void CellGroup::removeLastMoveOrder()
{
  if( movementQueue.empty() )
    return;

  pathHead.x -= movementQueue.back().x;
  pathHead.y -= movementQueue.back().y;
  movementQueue.pop_back();
}

// The default neighbor handler -- do nothing
void CellGroup::handleNeighbors (vector<CellGroup*> neighbors) {}

void CellGroup::move (Direction dir) {
  for( int i = 0; i < cells.size(); ++i )
    cells.at(i).move( dir );
}

void CellGroup::queueStandardMovementOrders () {
  Direction dir;
  cout << "SMO size " << standardMovementOrders.size() << endl;
  for( int i = 0; i < standardMovementOrders.size(); ++i ) {
    dir = standardMovementOrders[i];
    cout << "Queueing movement " << dir.x << ", " << dir.y << endl;
    movementQueue.push_back( dir );
    pathHead.x += dir.x;
    pathHead.y += dir.y;
  }
}

void CellGroup::issueMovementOrder ( Direction dir ) {
  movementQueue.push_back(dir);
  pathHead.x += dir.x;
  pathHead.y += dir.y;
}

// MOVEMENT OCCURS ON THE UPCYCLE
void CellGroup::upCycle () {
  if( movementQueue.empty() ) //If there are no queued movements
    if( standardMovementOrders.size() > 0 ) //If the unit has standard orders to follow (e.g. is AI)
      queueStandardMovementOrders();        //queue them
    else
      return; // Do not move if there are no movement orders and the unit is not AI

  if ( freeToMove )
    for (int i = 0; i < cells.size(); ++i)
      cells.at( i ).move( movementQueue.front() );
  else {
    pathHead.x -= movementQueue.front().x;
    pathHead.y -= movementQueue.front().y;
  }
  freeToMove = 1;

  movementQueue.pop_front();
  if( movementQueue.empty() ) //If there are no queued movements
    if( standardMovementOrders.size() > 0 ) //If the unit has standard orders to follow (e.g. is AI)
      queueStandardMovementOrders();        //queue them  
}

void CellGroup::downCycle () {}

vector<Location> CellGroup::getLocations () {
  locations.clear();
  for (int i = 0; i < cells.size(); i++)
    locations.push_back( cells.at(i).getGridLocation() );

  return locations;
}

vector<FloatPair> CellGroup::getScreenLocations () {
  vector<FloatPair> locs;
  for (int i = 0; i < cells.size(); ++i) {
    locs.push_back( cells[i].getScreenLocation() );
  }
  return locs;
}

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

int CellGroup::numOfMovements() {
  return movementQueue.size();
}

void CellGroup::setSMO ( vector<Direction> m ) {
  standardMovementOrders = m;
  queueStandardMovementOrders();
}

void CellGroup::setGridData (int w, int h, int t, int l) {
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setGridData( w, h, t, l );
  }
}

void CellGroup::setFreeToMove (int f) {
  freeToMove = f;
}
