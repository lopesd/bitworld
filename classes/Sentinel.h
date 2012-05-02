/** Sentinel.h
 *  The Sentinel is a security bit that detects anomalies within its line of sight.
 *  It zaps in a set sequence.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef SENTINEL_H
#define SENTINEL_H

#include "CellGroup.h"

#include <vector>

class Sentinel : public CellGroup {
  
 public:
  /** CONSTRUCTORS **/
  Sentinel (std::vector<Cell> cells);
  
  /** UTILITY FUNCTIONS **/
  
  //adds to orders
  void queueStandardActionOrders ();
  //zaps in downCycle 
  void downCycle ();
  //Draws tracer
  void makeAnimation ( std::vector<Location> locationsToZap );

  /** ACCESSORS **/
  Direction getDirection();

  /** MUTATORS **/
  void setStandardActionOrders ( std::vector<int> );
  
    //overloaded so can't be corrupted
  void dropResistance ( int n );
  
  //sets which way it is facing
  void setDirection(Direction d);
  
  //Checks if its a Sentinel and debugging
  std::string type () {return std::string("Sentinel");}

 private:
  std::vector<int> standardActionOrders; //1 for zap, 0 for hold
  std::deque<int> actionQueue;
  Direction dir; //way the sentinel is facing
  float rotation; // rotation of sprite due to sentinel orientation
  Location locationToZap; //where bit to be flagged is

};

#endif
