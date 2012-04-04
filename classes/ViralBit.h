#ifndef VIRAL_BIT_H
#define VIRAL_BIT_H

#include "CellGroup.h"

class ViralBit : public CellGroup {

 public:
  ViralBit  (std::vector<Cell> cells);
  ViralBit  (Cell cell);
  ViralBit  ();
  ~ViralBit ();

  std::string type () {return std::string("ViralBit");}

 private:

};

#endif
