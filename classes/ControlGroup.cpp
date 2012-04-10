/** ControlGroup.cpp
 *  A ControlGroup represents a "player" in the game. It is the base class for the AI and User specific groups.
 *  It is responsible for keeping track of a selected unit and directing given input to it.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "ControlGroup.h"

using namespace std;

/** CONSTRUCTORS **/
ControlGroup::ControlGroup (vector<CellGroup*> u) {
  units = u;
  selectedUnit = 0;

  for (int i = 0; i < units.size(); ++i) { // Initialize the CellGroups' controlGroups
    units.at(i)->controlGroup = this;
  }
}

/** UTILITY FUNCTIONS **/
void ControlGroup::take ( CellGroup* unitToTake ) {
  unitToTake->controlGroup = this;
  units.push_back( unitToTake );
}

void ControlGroup::forfeit ( CellGroup* unitToForfeit ) {
  if( selectedUnit = unitToForfeit ) selectedUnit = 0;
  for( int i = 0; i < units.size(); ++i )
    if( units[i] == unitToForfeit )
      units.erase( units.begin() + i );
}

/** ACCESSORS **/
CellGroup* ControlGroup::getSelectedUnit() {
  return selectedUnit;
}

/** MUTATORS **/
void ControlGroup::setLevel (Level* l) {
  level = l;
}

void ControlGroup::clearSelection () {
  selectedUnit = 0;
}
