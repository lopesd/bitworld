#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"
#include <map>

class WhiteBit : public CellGroup {

 public:
  WhiteBit (std::vector<Cell> cells);
  void addFlagged( CellGroup* );
  
  std::string type () {return std::string("WhiteBit");}
  
  void dropResistance ( int n ) {}; //Override the drop resistance function so the white bit cannot be corrupted
  Direction findMove ( std::map<Location, CellGroup*> grid );
  void findClosest (); 
  
 private:
  
 
};

#endif
