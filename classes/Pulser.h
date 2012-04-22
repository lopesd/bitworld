#ifndef PULSER_H
#define PULSER_H

#include "CellGroup.h"

#include <vector>

class Pulser : public CellGroup {
  
 public:
  Pulser (std::vector<Cell> cells);

  std::string type () {return std::string("Pulser");}

  //Pulsers don't handle neighbors, so use default empty neighbor handler
  void queueStandardActionOrders (int cycles); 
  int getRadius();
  std::vector<Event> downCycle ();
  void setRadius (int radius);

 private:
  std::vector<int> standardActionOrders; //1 for pulse, 0 for hold
  std::vector<int> actionQueue;
  int pulseRadius;
  int SAOCounter;

};

#endif
