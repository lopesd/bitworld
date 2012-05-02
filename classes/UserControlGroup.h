/** UserControlGroup.cpp
 *  The User Control Group handles interaction between the user and the units he controls.
 *  It keeps track of the selected unit and allows users to issue orders.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef USERCONTROLGROUP_H
#define USERCONTROLGROUP_H

#include <SFML/Graphics.hpp>

#include "ControlGroup.h"
#include "CellGroup.h"

class UserControlGroup : public ControlGroup {

 public:
  /** CONSTRUCTORS **/
  UserControlGroup ( std::vector<CellGroup*> );

  /** UTILITY FUNCTIONS **/
  // Start the turn
  void startTurn   ();

  // Input handlers
  void handleInput ( CellGroup* clickedUnit );
  void handleInput ( Direction dir );
  void handleInput ( sf::Key::Code );

  // Take a unit into my control
  void take ( CellGroup* unitTotake );

 private:
  // Toggle selection of a unit on or off
  void toggleSelection ( CellGroup* unit );
};

#endif
