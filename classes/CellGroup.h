/** CellGroup.h
 *  A CellGroup is a group of cells (usually one) that defines a single unit.
 *  It is responsible for drawing and moving its cells. It is the base class for specific kinds of units.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/


#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "Cell.h"
#include "ControlGroup.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <deque>

class ControlGroup; //Forward declaration

class CellGroup {

 public:
  //CONSTRUCTORS
  CellGroup (std::vector<Cell> cells);
  CellGroup (Cell cells);
  CellGroup ();

  //CONTROL FUNCTIONS
  virtual void draw ( sf::RenderWindow& screen );
  virtual void move (Direction dir);
  virtual void handleNeighbors (std::vector<CellGroup*> neighbors);
  virtual void queueStandardMovementOrders ();
  virtual void issueMovementOrder (Direction dir);
  virtual void removeLastMoveOrder();
  virtual void upCycle   ();
  virtual void downCycle ();

  //ACCESSORS
  virtual std::vector<Location>  getLocations (); //returns location or locations in a vector
  virtual std::vector<FloatPair> getScreenLocations (); //returns positions on screen
  virtual FloatPair              getMiddle ();
  virtual Direction              getMovement(int num);
  virtual int                    numOfMovements ();
  virtual FloatPair              getPathHead ();
  virtual int                    getWeight ();
  virtual std::string            type () = 0; //return the type of the unit

  //MUTATORS
  virtual void setSMO      (std::vector<Direction>);
  virtual void setGridData (int, int, int, int);
  virtual void setFreeToMove (int);

  //PUBLIC MEMBER VARIABLES
  ControlGroup* controlGroup;
  std::string   CGGroupName;

 protected:
  std::vector<Cell> cells;
  std::vector<Location> locations;
  std::vector<Direction> standardMovementOrders;
  std::deque<Direction> movementQueue;
  int SMOCounter;

  // Tracks the current projected destination, starts at the middle of the unit
  FloatPair pathHead;

  int weight; // The weight, or precedence, of the unit over others
  int freeToMove; //Indicates whether the unit can move (eg no collisions), set to 0 by default

};

#endif
