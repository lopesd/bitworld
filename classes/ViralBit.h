#ifndef VIRAL_BIT_H
#define VIRAL_BIT_H

#include "CellGroup.h"

class ViralBit : public CellGroup {
  
 public:
  ViralBit  (std::vector<Cell> cells);

  std::string type () {return std::string("ViralBit");} //for debugging

  void downCycle (); 

 private:

};

#endif
