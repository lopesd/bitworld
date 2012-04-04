#include "Pulser.h"

using namespace std;

Pulser::Pulser () {}

Pulser::Pulser (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "pulser_bit.png" );
  }
}

Pulser::Pulser (Cell cell) : CellGroup(cell) {}

void Pulser::queueStandardActionOrders (int cycles) {
  for (int i = 0; i < cycles; ++i, ++SAOCounter) {
    actionQueue.push_back( standardActionOrders.at( SAOCounter ));
    if (SAOCounter >= standardActionOrders.size())
      SAOCounter = 0;
  }
}
