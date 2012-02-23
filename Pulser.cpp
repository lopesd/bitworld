// Pulser.cpp
#include "Pulser.h"

Pulser::Pulser () {}

Pulser::~Pulser () {}

void queueStandardOrders (int cycles) {
  for (int i = 0; i < cycles; ++i, ++standardOrderCounter) {
    actionQueue.push_back( standardOrders.at( standardOrderCounter ));
    if (standardOrderCounter >= standardOrders.size())
      standardOrderCounter = 0;
  }
}
