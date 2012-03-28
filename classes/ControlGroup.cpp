#include "ControlGroup.h"

using namespace std;

ControlGroup::ControlGroup (vector<CellGroup*> u) {
  units = u;
  selectedUnit = 0;

  for (int i = 0; i < units.size(); ++i) { // Initialize the CellGroups' controlGroups
    units.at(i)->controlGroup = this;
  }
}

CellGroup* ControlGroup::getSelectedUnit() {
  return selectedUnit;
}

