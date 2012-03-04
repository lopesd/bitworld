#ifndef USERCONTROLGROUP_H
#define USERCONTROLGROUP_H

#include "ControlGroup.h"
#include "CellGroup.h"

class UserControlGroup : public ControlGroup {

 public:
  UserControlGroup (vector<CellGroup*>);
  void handleInput (CellGroup* clickedUnit);
  void handleInput (Direction dir);

 private:
  CellGroup* selectedUnit;

  void toggleSelection (CellGroup* unit);


};

#endif
