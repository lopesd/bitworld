#include "CellGroup.h"

#include <iostream> // FOR TESTING

using namespace std;

CellGroup::CellGroup ( vector<Cell> c ) {
  SMOCounter = 0;
  cells = c;
  controlGroup = 0;
  pathHead = getMiddle();
}

CellGroup::CellGroup (Cell c) {
  SMOCounter = 0;
  cells.push_back( c );
  controlGroup = 0;
  pathHead = getMiddle();
}

CellGroup::CellGroup () {
  SMOCounter = 0;
}

CellGroup::~CellGroup () {
  cout << "CellGroup Deconstructor" << endl;
}

void CellGroup::setGridData (int w, int h, int t, int l) {
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setGridData( w, h, t, l );
  }
}

void CellGroup::draw ( sf::RenderWindow& screen ) {
  for (vector<Cell>::iterator i = cells.begin(); i != cells.end(); ++i)
    i->draw( screen );
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

FloatPair CellGroup::getPathHead () {
  return pathHead;
}

// The default neighbor handler -- do nothing
void CellGroup::handleNeighbors (vector<CellGroup*> neighbors) {}

void CellGroup::move (Direction dir) {
  for (int i = 0; i < cells.size(); ++i)
    cells.at(i).move( dir );
}

void CellGroup::queueStandardMovementOrders (int cycles) {}

void CellGroup::issueMovementOrder (Direction dir) {
  movementQueue.push_back(dir);
  pathHead.x += dir.x;
  pathHead.y += dir.y;
}

// MOVEMENT OCCURS ON THE UPCYCLE
void CellGroup::upCycle ()
{
  if ( movementQueue.empty() )
    return; // Do not move if there are no movement orders

  for (int i = 0; i < cells.size(); ++i)
    cells.at( i ).move( movementQueue.front() );

  movementQueue.pop_front();
}

void CellGroup::downCycle () {}

vector<Location> CellGroup::getLocations ()
{
  locations.clear();
  for (int i = 0; i < cells.size(); i++)
    locations.push_back( cells.at(i).getGridLocation() );

  return locations;
}

Direction CellGroup::getMovement(int num)
{
  return movementQueue[num];
}

int CellGroup::numOfMovements()
{
  return movementQueue.size();
}

void CellGroup::removeLastMoveOrder()
{
  if(movementQueue.empty())
    return;

  pathHead.x -= movementQueue.back().x;
  pathHead.y -= movementQueue.back().y;
  movementQueue.pop_back();
}
