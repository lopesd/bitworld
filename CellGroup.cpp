#include "CellGroup.h"

#include <iostream> // FOR TESTING

CellGroup::CellGroup (vector<Cell*> c) {
  std::cout << "Size of cell vector: " << c.size() << endl;
  SMOCounter = 0;
  cells = c;
  controlGroup = 0;
}

CellGroup::CellGroup (Cell* c) {
  SMOCounter = 0;
  cells.push_back( c );
}

CellGroup::~CellGroup () {}

void CellGroup::drawMe () {

  for (vector<Cell*>::iterator i = cells.begin(); i != cells.end(); ++i) {
    (*i)->draw();
  }

}

// The default neighbor handler -- do nothing
void CellGroup::handleNeighbors (vector<CellGroup*> neighbors) {}

void CellGroup::move (Direction dir) {
  for (int i = 0; i < cells.size(); ++i) {
    cells.at(i)->move( dir );
  }
}

void CellGroup::queueStandardMovementOrders (int cycles) {
  
}

void CellGroup::issueMovementOrder (Direction dir) {

  movementQueue.push_back(dir);

}

// MOVEMENT OCCURS ON THE UPCYCLE
void CellGroup::upCycle () {
  
  if ( movementQueue.empty() ) return; // Do not move if there are no movement orders
  
  for (int i = 0; i < cells.size(); ++i) {
    cells.at( i )->move( movementQueue.front() );
  }
  
  movementQueue.pop_front();
}

void CellGroup::downCycle () {

}


vector<Location> CellGroup::getLocations () {
  vector<Location> locations;
  for (int i = 0; i < cells.size(); i++) {
    locations.push_back( cells.at(i)->getGridLocation() );
    cout << "Returning grid location " << i << endl;
  }

  return locations;
}

char CellGroup::getImage () {
  return 'O';
}
