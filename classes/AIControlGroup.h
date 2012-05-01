/** AIControlGroup.h
 *  The AI Control Group is responsible for decisions regarding the AI.
 *  It is very simple, because any decisions regarding unit movement is made by the unit itself.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef AICONTROLGROUP_H
#define AICONTROLGROUP_H

#include "ControlGroup.h"
#include "CellGroup.h"

class AIControlGroup : public ControlGroup {

 public:
  /** CONSTRUCTORS **/
  AIControlGroup (std::vector<CellGroup*>);
  
  /** UTILITY FUNCTIONS **/
  // Nothing really needs to be done in the AI's turn
  void startTurn   ();
  
  // No input needs to be handled, so make these functions empty
  void handleInput ( CellGroup* clickedUnit ) {}
  void handleInput ( Direction dir ) {}
  void handleInput ( sf::Key::Code ) {}

  // Place a unit under this Control Group's command
  void take ( CellGroup* unitToTake );
  
 private:
  
};

#endif
