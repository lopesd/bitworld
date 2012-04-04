#include "UserControlGroup.h"

#include <iostream> // For debugging, remove later

using namespace std;

UserControlGroup::UserControlGroup (vector<CellGroup*> c)
:ControlGroup(c) {
  selectedUnit = 0;
}

void UserControlGroup::startTurn () {
  // Do nothing -- await more input
}

void UserControlGroup::handleInput (CellGroup* clickedUnit)
{
  if( !clickedUnit || clickedUnit->controlGroup != this )
    selectedUnit = 0;
  else
    toggleSelection( clickedUnit );
}

void UserControlGroup::handleInput (Direction dir) {
  if( selectedUnit ) selectedUnit->issueMovementOrder (dir);
}

void UserControlGroup::handleInput (sf::Key::Code keyPressed) {
  if( keyPressed == sf::Key::Back ) {
    if( selectedUnit ) selectedUnit->removeLastMoveOrder();
  }
  else if( keyPressed == sf::Key::Space ) {
    level->controlGroupDone();
  }
}

void UserControlGroup::toggleSelection (CellGroup* unit) {
  if (selectedUnit == unit)
    selectedUnit = 0;
  else
    selectedUnit = unit;
}
