#include "ControlGroup.h"

#include <iostream> // FOR TESTING

ControlGroup::ControlGroup (vector<CellGroup*> u) {
  units = u;
  selectedUnit = 0;

  for (int i = 0; i < units.size(); ++i) { // Initialize the CellGroups' controlGroups
    std::cout << "Setting CG" << endl;
    units.at(i)->controlGroup = this;
  }
}

CellGroup* ControlGroup::getSelectedUnit()
{
  return selectedUnit;
}

