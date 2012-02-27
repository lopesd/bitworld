#include "UserControlGroup.h"

#include <iostream> // For debugging, remove later

UserControlGroup::UserControlGroup (vector<CellGroup*> c) : ControlGroup(c) {
  selectedUnit = 0;
}

void UserControlGroup::handleInput (CellGroup* clickedUnit) {

  if (!clickedUnit) {
    std::cout << "deselecting" << endl;
    selectedUnit = 0;
  }
  else if (clickedUnit->controlGroup == this) {  //check if CG owns the object
    std::cout << "toggling selection" << endl;
    toggleSelection (clickedUnit);
    if (selectedUnit) std::cout << "a unit is selected" << endl;
  }

}

void UserControlGroup::handleInput (Direction dir) {
  if (selectedUnit) {
    selectedUnit->issueMovementOrder (dir);
    cout << "issuing movement order" << endl;
  }
}

void UserControlGroup::toggleSelection (CellGroup* unit) {
  if (selectedUnit == unit)
    selectedUnit = 0;
  else 
    selectedUnit = unit;
}
