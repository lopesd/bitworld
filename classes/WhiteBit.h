#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"

class WhiteBit : public CellGroup {

 public:
  WhiteBit (std::vector<Cell> cells);

  std::string type () {return std::string("WhiteBit");}
  
  void dropResistance ( int n ) {}; //Override the drop resistance function so the white bit cannot be corrupted

 private:

};

#endif
