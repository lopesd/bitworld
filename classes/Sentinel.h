/** Sentinel.h
 *  The Sentinel is a security bit that detects anomalies within its line of sight.
 *  It pulses in a set sequence.
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
  void queueStandardActionOrders (); 
  void downCycle ();
  void makeAnimation ( std::vector<Location> locationsToZap );

  /** ACCESSORS **/
	Direction getDirection();

  /** MUTATORS **/
  void setStandardActionOrders ( std::vector<int> );
  void setDirection(Direction d);
  std::string type () {return std::string("Sentinel");}

 private:
  std::vector<int> standardActionOrders; //1 for pulse, 0 for hold
  std::deque<int> actionQueue;
  int SAOCounter;
	Direction dir; //way the sentinel is facing
	Location locationToZap; //where bit to be flagged is
};

#endif
