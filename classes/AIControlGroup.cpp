/** AIControlGroup.cpp
 *  The AI Control Group is responsible for decisions regarding the AI.
 *  It is very simple, because any decisions regarding unit movement is made by the unit itself.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "AIControlGroup.h"

#include <iostream>

using namespace std;

/** CONSTRUCTORS **/
AIControlGroup::AIControlGroup (vector<CellGroup*> c) : ControlGroup(c) {
  selectedUnit = 0;
  player = 0;
}

/** UTILITY **/
// Nothing really needs to be done in the AI's turn
void AIControlGroup::startTurn () {
  level->controlGroupDone();
}

// Place a unit under this Control Group's command
void AIControlGroup::take ( CellGroup* unitToTake ) {
  ControlGroup::take( unitToTake ); //Call base class function
  unitToTake->CGGroupName = "AI";
}
