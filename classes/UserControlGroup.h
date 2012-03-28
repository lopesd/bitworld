#ifndef USERCONTROLGROUP_H
#define USERCONTROLGROUP_H

#include <SFML/Graphics.hpp>

#include "ControlGroup.h"
#include "CellGroup.h"

class UserControlGroup : public ControlGroup {

 public:
  UserControlGroup (std::vector<CellGroup*>);
  void handleInput (CellGroup* clickedUnit);
  void handleInput (Direction dir);
  void handleInput (sf::Key::Code);

 private:
  void toggleSelection (CellGroup* unit);
};

#endif
