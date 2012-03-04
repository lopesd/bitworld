// DEPRECATED CODE, MIGHT BE REVIVED LATER

#ifndef MAP_H
#define MAP_H

#include "CellGroup.h"
#include "structures"
#include <vector>

class CellGroup;

class Map {

 public:
  Map ();
  
  vector<CellGroup*> findNeighbors (CellGroup* unit);


 private:
  vector<CellGroup*> units;
  vector<vector<CellGroup*> > grid;  
  
};

#endif
