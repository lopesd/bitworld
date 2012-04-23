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
  if( selectedUnit ) {

    vector<Location> flocs = selectedUnit->getLocationPathHeads();
    for( int i = 0; i < flocs.size(); ++i ) { //Check to see if the movement will be off-screen
      Location floc = flocs[i] + dir;
      if( (floc.x < 0) || (floc.y < 0) || 
	  (floc.x >= level->getWidth()) || (floc.y >= level->getHeight()) )
	return; //Do not issue an off-screen movement.
    }

    selectedUnit->issueMovementOrder (dir); //issue the order
  }
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

void UserControlGroup::take ( CellGroup* unitToTake ) {
  ControlGroup::take( unitToTake );
  unitToTake->setSMO( vector<Direction>() );
  unitToTake->clearMovementQueue();
  unitToTake->CGGroupName = "user";
}
