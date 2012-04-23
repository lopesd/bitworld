#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"
#include <map>

#include <set>

class WhiteBit : public CellGroup {

 public:
  WhiteBit (std::vector<Cell> cells);
  void upCycle ();
  
  void dropResistance ( int n ) {}; //Override the drop resistance function so the white bit cannot be corrupted
  Direction findMove ( std::map<Location, CellGroup*> grid );
  void findClosest ( std::set<CellGroup*> flaggedBits ); 

  std::string type () {return std::string("WhiteBit");}

 private:
  CellGroup* chosen;
  int movementDistance;
};

#endif
