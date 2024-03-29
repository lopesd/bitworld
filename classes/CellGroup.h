/** CellGroup.h
 *  A CellGroup is a group of cells (usually one) that defines a single unit.
 *  It is responsible for drawing and moving its cells. It is the base class for specific kinds of units.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/


#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "Cell.h"
#include "ControlGroup.h"
#include "Event.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <deque>

class ControlGroup; //Forward declaration
class Event;

class CellGroup {

 public:
  //CONSTRUCTORS
  CellGroup (std::vector<Cell> cells);

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
  virtual std::vector<Location>  getLocationPathHeads ();
  virtual int                    getWeight ();
  virtual int                    getResistance ();
  virtual std::string            type () = 0; //return the type of the unit

  //MUTATORS
  virtual void setLocation ( Location newLoc );
  virtual void setSMO      (std::vector<Direction>);
  virtual void clearMovementQueue ();
  virtual void setGridData (int, int, int, int);
  virtual void setFreeToMove (int);
  virtual void setMaxResistance (int);
  virtual void dropResistance ( int n = 1 ); //can be overloaded for a unit that can't be corrupted
  virtual void resetResistance (); //can be overloaded for, say, a unit that doesn't recover resistance
  virtual void corrupt ();

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
  int maxResistance, resistance; //Resistance to corruption by opponent viral bits
  int resistanceDropped;   //Indicates if resistance has dropped this turn
  int freeToMove; //Indicates whether the unit can move (eg no collisions), set to 1 by default

};

#endif
