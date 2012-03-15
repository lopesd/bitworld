#include "Pulser.h"

Pulser::Pulser () {}

Pulser::Pulser (vector<Cell> cells) : CellGroup(cells) {}

Pulser::Pulser (Cell cell) : CellGroup(cell) {}

void Pulser::queueStandardActionOrders (int cycles) {
  for (int i = 0; i < cycles; ++i, ++SAOCounter) {
    actionQueue.push_back( standardActionOrders.at( SAOCounter ));
    if (SAOCounter >= standardActionOrders.size())
      SAOCounter = 0;
  }
}
