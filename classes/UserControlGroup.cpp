#include "UserControlGroup.h"

#include <iostream> // For debugging, remove later

using namespace std;

UserControlGroup::UserControlGroup (vector<CellGroup*> c)
:ControlGroup(c)
{
  selectedUnit = 0;
}

void UserControlGroup::handleInput (CellGroup* clickedUnit)
{

  if (!clickedUnit)
  {
    cout << "deselecting" << endl;
    selectedUnit = 0;
  }
  else
    if (clickedUnit->controlGroup == this) //check if CG owns the object
    {
      cout << "toggling selection" << endl;
      toggleSelection (clickedUnit);

      if (selectedUnit)
        cout << "a unit is selected" << endl;
    }
}

void UserControlGroup::handleInput (Direction dir)
{
  if (selectedUnit)
  {
    selectedUnit->issueMovementOrder (dir);
    cout << "issuing movement order" << endl;
  }
}

void UserControlGroup::toggleSelection (CellGroup* unit)
{
  if (selectedUnit == unit)
    selectedUnit = 0;
  else
    selectedUnit = unit;
}

