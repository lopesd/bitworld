#ifndef PULSER_H
#define PULSER_H

#include "CellGroup.h"

class Pulser : public CellGroup {
  
 public:
  Pulser (vector<Cell> cells);
  Pulser (Cell cell);
  Pulser ();

  //Pulsers don't handle neighbors, so use default empty neighbor handler
  void queueStandardActionOrders (int cycles);

 private:
  vector<int> standardActionOrders; //1 for pulse, 0 for hold
  vector<int> actionQueue;
  int SAOCounter;

};

#endif
