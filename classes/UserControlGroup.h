#ifndef USERCONTROLGROUP_H
#define USERCONTROLGROUP_H

#include <SFML/Graphics.hpp>

#include "ControlGroup.h"
#include "CellGroup.h"

class UserControlGroup : public ControlGroup {

 public:
  UserControlGroup (std::vector<CellGroup*>);

  void startTurn   ();
  void handleInput (CellGroup* clickedUnit);
  void handleInput (Direction dir);
  void handleInput (sf::Key::Code);

  void take ( CellGroup* unitTotake );

 private:
  void toggleSelection (CellGroup* unit);
};

#endif
