#include "Pulser.h"

using namespace std;

Pulser::Pulser (vector<Cell> c) : CellGroup(c) {
  weight = 3;
  maxResistance = resistance = 5;
  vector<string> imgs;
  imgs.push_back( "pulser_bit2.png" );
  imgs.push_back( "pulser_center.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs, PULSER );
  }
}

void Pulser::queueStandardActionOrders (int cycles) {
  for (int i = 0; i < cycles; ++i, ++SAOCounter) {
    actionQueue.push_back( standardActionOrders.at( SAOCounter ));
    if (SAOCounter >= standardActionOrders.size())
      SAOCounter = 0;
  }
}
