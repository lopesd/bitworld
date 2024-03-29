/** ControlGroup.cpp
 *  A ControlGroup represents a "player" in the game. It is the base class for the AI and User specific groups.
 *  It is responsible for keeping track of a selected unit and directing given input to it.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "ControlGroup.h"
#include <cstring>

using namespace std;

/** CONSTRUCTORS **/
ControlGroup::ControlGroup ( vector<CellGroup*> u ) {
  units = u;
  selectedUnit = 0;
  if( !units.empty() ) {
    selectedUnit = units[0]; // select the first unit
  }

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
  if( selectedUnit == unitToForfeit ) selectedUnit = 0;
  for( int i = 0; i < units.size(); ++i ) // Clumsily remove the unit from my vector
    if( units[i] == unitToForfeit )
      units.erase( units.begin() + i );
}
 
void ControlGroup::clearSelection () {
  selectedUnit = 0;
}

/** ACCESSORS **/
CellGroup* ControlGroup::getSelectedUnit() {
  return selectedUnit;
}

int ControlGroup::getUnitsSize() {
  return units.size();
}

vector<CellGroup*> ControlGroup::getUnits() {
  return units;
}

int ControlGroup::getPlayer() {
  return player;
}

/** MUTATORS **/
void ControlGroup::setLevel (Level* l) {
  level = l;
}
