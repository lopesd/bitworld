#ifndef PULSER_H
#define PULSER_H

#include "CellGroup.h"

class Pulser : public CellGroup {
  
 public:
  Pulser (std::vector<Cell> cells);
  Pulser (Cell cell);
  Pulser ();

  //Pulsers don't handle neighbors, so use default empty neighbor handler
  void queueStandardActionOrders (int cycles);

 private:
  std::vector<int> standardActionOrders; //1 for pulse, 0 for hold
  std::vector<int> actionQueue;
  int SAOCounter;

};

#endif
