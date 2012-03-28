#ifndef AICONTROLGROUP_H
#define AICONTROLGROUP_H

#include "ControlGroup.h"
#include "CellGroup.h"

class AIControlGroup : public ControlGroup {

 public:
  AIControlGroup (std::vector<CellGroup*>);
  void handleInput (CellGroup* clickedUnit) {}
  void handleInput (Direction dir) {}

 private:
  //void toggleSelection (CellGroup* unit);
};

#endif
