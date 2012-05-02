/** UserControlGroup.cpp
 *  The User Control Group handles interaction between the user and the units he controls.
 *  It keeps track of the selected unit and allows users to issue orders.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "UserControlGroup.h"

#include <iostream> // For debugging, remove later

using namespace std;

/** CONSTRUCTORS **/
UserControlGroup::UserControlGroup ( vector<CellGroup*> c )
: ControlGroup(c) {
  if( !units.empty() )
    selectedUnit = units[0]; // select the first unit
  player = 1;
}

/** UTILITY FUNCTIONS **/
// Start user's turn
void UserControlGroup::startTurn () {
  // Do nothing -- await more input from level
}

// Input handlers
void UserControlGroup::handleInput ( CellGroup* clickedUnit ) {
  if( !clickedUnit || clickedUnit->controlGroup != this )
    selectedUnit = 0;
  else
    toggleSelection( clickedUnit );
}

// We are given a direction -- issue the order to our selected unit
void UserControlGroup::handleInput (Direction dir) {

  if( selectedUnit ) { // If there is a unit selected...

    // Don't queue more movements than there are cycles in a period.
    if( selectedUnit->numOfMovements() >= level->getCyclesPerPeriod() ) return;

    //Check to see if the movement will be off-screen for any part of the unit
    vector<Location> flocs = selectedUnit->getLocationPathHeads();
    for( int i = 0; i < flocs.size(); ++i ) { 
      Location floc = flocs[i] + dir;
      if( (floc.x < 0) || (floc.y < 0) || 
	  (floc.x >= level->getWidth()) || (floc.y >= level->getHeight()) )
	return; //Do not issue an off-screen movement.
    }

    selectedUnit->issueMovementOrder (dir); //issue the order
  }

}

// Handle backspaces and spaces
void UserControlGroup::handleInput (sf::Key::Code keyPressed) {
  if( keyPressed == sf::Key::Back ) {
    if( selectedUnit ) selectedUnit->removeLastMoveOrder();
  }
  else if( keyPressed == sf::Key::Space ) {
    level->controlGroupDone();
  }
}

// Take a unit into my control
void UserControlGroup::take ( CellGroup* unitToTake ) {
  ControlGroup::take( unitToTake ); // base class function
  unitToTake->setSMO( vector<Direction>() ); // clear their standard movement orders
  unitToTake->clearMovementQueue(); 
  unitToTake->CGGroupName = "user";
  // Select the unit if it has priority over the already selected unit
  if( !selectedUnit ) selectedUnit = unitToTake;
  else if( selectedUnit->getWeight() < unitToTake->getWeight() ) selectedUnit = unitToTake;
}

// Toggle selection of a unit on or off
void UserControlGroup::toggleSelection (CellGroup* unit) {
  if (selectedUnit == unit)
    selectedUnit = 0;
  else
    selectedUnit = unit;
}
