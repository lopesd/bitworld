// Pulser.h
#ifndef PULSER_H
#define PULSER_H

#include "CellGroup.h"

class Pulser : CellGroup {

 public:
  Pulser ();
  ~Pulser ();

  //Pulsers don't handle neighbors, so use default empty function
  void queueStandardOrders (int cycles);

 private:
  vector standardOrders; //1 for pulse, 0 for hold
  vector actionQueue;

};

#endif
