#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "Cell.h"
#include "ControlGroup.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <deque>

using namespace std;

class ControlGroup; //Forward declaration

class CellGroup {

 public:
  CellGroup (vector<Cell> cells);
  CellGroup (Cell cells);
  CellGroup ();
  ~CellGroup ();

  virtual void draw ( sf::RenderWindow& screen );
  virtual void move (Direction dir);
  virtual void handleNeighbors (vector<CellGroup*> neighbors);
  virtual void queueStandardMovementOrders (int cycles);
  virtual void issueMovementOrder (Direction dir);
  virtual void removeLastMoveOrder();
  virtual void upCycle   ();
  virtual void downCycle ();

  virtual vector<Location> getLocations (); //returns location or locations in a vector
  virtual FloatPair        getMiddle ();
  virtual Direction        getMovement(int num);
  virtual int              numOfMovements ();
  virtual FloatPair        getPathHead ();

  virtual void setGridData (int, int, int, int);
  
  ControlGroup* controlGroup;

 protected:
  vector<Cell> cells;
  vector<Location> locations;
  vector<Direction> standardMovementOrders;
  deque<Direction> movementQueue;
  int SMOCounter;

  // Tracks the current projected destination, starts at the middle of the unit
  FloatPair pathHead;

};

#endif
