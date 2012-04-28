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
  void downCycle ();
  void makeAnimation ( std::vector<Location> locationsToPulse );

  /** ACCESSORS **/
  int getRadius();

  /** MUTATORS **/
  void setRadius (int radius);
  void setStandardActionOrders ( std::vector<int> );
  
  std::string type () {return std::string("Pulser");}

 private:
  std::vector<int> standardActionOrders; //1 for pulse, 0 for hold
  std::deque<int> actionQueue;
  int pulseRadius;
  int SAOCounter;

};

#endif
