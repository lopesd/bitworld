#include "AIControlGroup.h"

#include <iostream>

using namespace std;

AIControlGroup::AIControlGroup (vector<CellGroup*> c) : ControlGroup(c) {
  selectedUnit = 0;
  player = 0;
}

void AIControlGroup::startTurn () {
  level->controlGroupDone();
}

void AIControlGroup::take ( CellGroup* unitToTake ) {
  ControlGroup::take( unitToTake );
  unitToTake->CGGroupName = "AI";
}
