/** Pulser.h
 *  The pulser is a security bit that detects anomalies within its range.
 *  It pulses in a set sequence.
 */

#ifndef PULSER_H
#define PULSER_H

#include "CellGroup.h"

#include <vector>

class Pulser : public CellGroup {
  
 public:
  /** CONSTRUCTORS **/
  Pulser (std::vector<Cell> cells);
  
  /** UTILITY FUNCTIONS **/
  void queueStandardActionOrders (); 
  
  //pulses in downCycle
  void downCycle ();

  /** ACCESSORS **/
  int getRadius();

  /** MUTATORS **/  
  //sets pulse radius
  void setRadius (int radius);
  
    //overloaded so can't be corrupted
  void dropResistance ( int n );
  
  //sets the orders for the pulser 
  void setStandardActionOrders ( std::vector<int> );
  
  //Used for debugging and checking type of bit
  std::string type () {return std::string("Pulser");}

 private:
  std::vector<int> standardActionOrders; //1 for pulse, 0 for hold
  std::deque<int> actionQueue;
  int pulseRadius; //how far it pulses

};

#endif
