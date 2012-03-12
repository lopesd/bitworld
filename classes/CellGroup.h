#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "Cell.h"
#include "ControlGroup.h"

#include <vector>
#include <deque>

using namespace std;


class ControlGroup; //Forward declaration

class CellGroup {

 public:
  CellGroup (vector<Cell*> cells);
  CellGroup (Cell* cells);
  ~CellGroup ();

  virtual void drawMe ();
  virtual char getImage (); // TEMPORARY
  virtual void move (Direction dir);
  virtual void handleNeighbors (vector<CellGroup*> neighbors);
  virtual void queueStandardMovementOrders (int cycles);
  virtual void issueMovementOrder (Direction dir);
  virtual void upCycle   ();
  virtual void downCycle ();
  virtual vector<Location>& getLocations (); //returns location or locations in a vector

  ControlGroup* controlGroup;

 private:
  vector<Cell*> cells;

  vector<Location> locations;
  vector<Direction> standardMovementOrders;
  deque<Direction> movementQueue;
  int SMOCounter;
};

#endif