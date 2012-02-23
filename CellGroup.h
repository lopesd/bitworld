// CellGroup.h
#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "GridCell.h"

using namespace std;

class CellGroup {

 public:
  CellGroup ();
  ~CellGroup ();

  virtual void drawMe ();
  virtual void move (); //Define system for keeping track of directions. Pair object?
  virtual void handleNeighbors (vector neighbors);
  virtual void queueStandardOrders (int cycles) = 0; //Must be overloaded
  virtual vector<Location> getLocations //should they contain information on their location somehow...?
  //They could access their cells for that, or query the map...
  //Seems like they don't need that info, do they? Not too readily, that is.
  //They can just iterate through their list of cell objects, i think. (); //returns location or locations in a vector

  //virtual vector getLocations (); //returns location or locations in a vector

 private:
  vector cells;

  //vector standardOrders; //need some way to pack orders?
  //vector actionQueue;
  int standardOrderCounter;

  //should they contain information on their location somehow...?
  //They could access their cells for that, or query the map...
  //Seems like they don't need that info, do they? Not too readily, that is.
  //They can just iterate through their list of cell objects, i think.

};

#endif
