#ifndef LEVEL_H
#define LEVEL_H

#include "ControlGroup.h"
#include "CellGroup.h"
#include "structures.h"
#include <vector>
#include <map>


class Level {

 public:
  Level (vector<ControlGroup*>, vector<CellGroup*> units, int width=10, int height=10, int cpp=3);

  void display     ();
  void handleInput (Location  loc);
  void handleInput (Direction dir);
  void run         ();

  vector<CellGroup*> findNeighbors (CellGroup*);

 private:
  ControlGroup*                activeGroup;
  vector<ControlGroup*>        controlGroups;
  vector<CellGroup*>           units;
  map<Location, CellGroup*>    grid;
  int width, height;
  int cyclesPerPeriod;

  void updateGrid();

};

#endif
