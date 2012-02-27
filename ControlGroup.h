#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

#include "CellGroup.h"
#include "structures.h"
#include <vector>


class CellGroup; //Forward declaration

class ControlGroup {
  
 public:
  ControlGroup (vector<CellGroup*>);
  
  virtual void handleInput (Direction dir) = 0;
  virtual void handleInput (CellGroup* unit) = 0;

 private:
  vector<CellGroup*> units;
  CellGroup* selectedUnit;

};

#endif
