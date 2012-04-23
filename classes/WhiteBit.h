#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"

#include <set>

class WhiteBit : public CellGroup {

 public:
  WhiteBit (std::vector<Cell> cells);
  void addFlagged( CellGroup* );
  void findClosest(set<CellGroup*>& flaggedBits);
  Direction findMove();
  void upCycle ();
  std::string type () {return std::string("WhiteBit");}
  
  void dropResistance ( int n ) {}; //Override the drop resistance function so the white bit cannot be corrupted
  

 private:
  CellGroup* chosen;
  int movementDistance;
};

#endif
