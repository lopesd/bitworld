#include "AIControlGroup.h"

#include <iostream>

using namespace std;

AIControlGroup::AIControlGroup (vector<CellGroup*> c) : ControlGroup(c) {
  selectedUnit = 0;
}

void AIControlGroup::startTurn () {
  //Issue orders
  /*
  for( int i = 0; i < units.size(); ++i) {
    units[i]->queueStandardMovementOrders;
    }*/
  cout << "Ending turn." << endl;
  level->controlGroupDone();
}

void AIControlGroup::take ( CellGroup* unitToTake ) {
  ControlGroup::take( unitToTake );
  unitToTake->CGGroupName = "AI";
}
