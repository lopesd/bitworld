#include "CellGroup.h"

#include <iostream> // FOR TESTING

<<<<<<< HEAD
CellGroup::CellGroup (vector<Cell> c) {
=======
CellGroup::CellGroup (vector<Cell*> c)
{
  std::cout << "Size of cell vector: " << c.size() << endl;
>>>>>>> a16d182554ed6141604fb84d38f467cc60a51c2a
  SMOCounter = 0;
  cells = c;
  controlGroup = 0;
}

<<<<<<< HEAD
CellGroup::CellGroup (Cell c) {
=======
CellGroup::CellGroup (Cell* c)
{
>>>>>>> a16d182554ed6141604fb84d38f467cc60a51c2a
  SMOCounter = 0;
  cells.push_back( c );
  controlGroup = 0;
}

CellGroup::CellGroup () {
  SMOCounter = 0;
}

CellGroup::~CellGroup ()
{
}

<<<<<<< HEAD
  for (vector<Cell>::iterator i = cells.begin(); i != cells.end(); ++i)
    i->draw();

=======
void CellGroup::drawMe ()
{
  for (vector<Cell*>::iterator i = cells.begin(); i != cells.end(); ++i)
    (*i)->draw();
>>>>>>> a16d182554ed6141604fb84d38f467cc60a51c2a
}

// The default neighbor handler -- do nothing
void CellGroup::handleNeighbors (vector<CellGroup*> neighbors)
{
}

void CellGroup::move (Direction dir)
{
  for (int i = 0; i < cells.size(); ++i)
    cells.at(i).move( dir );
}

void CellGroup::queueStandardMovementOrders (int cycles)
{
}

void CellGroup::issueMovementOrder (Direction dir)
{
  movementQueue.push_back(dir);
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

void CellGroup::downCycle ()
{
}

<<<<<<< HEAD

vector<Location> CellGroup::getLocations ()
=======
vector<Location>& CellGroup::getLocations ()
>>>>>>> a16d182554ed6141604fb84d38f467cc60a51c2a
{
  locations.clear();
  for (int i = 0; i < cells.size(); i++)
    locations.push_back( cells.at(i).getGridLocation() );

  return locations;
}

char CellGroup::getImage ()
{
  return 'O';
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

  movementQueue.pop_back();
}
