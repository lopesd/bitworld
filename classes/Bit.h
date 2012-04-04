#ifndef BIT_H
#define BIT_H

#include "CellGroup.h"

class Bit : public CellGroup {

 public:
  Bit (std::vector<Cell> cells);
  Bit (Cell cell);
  Bit ();
  ~Bit();

  std::string type ();

 private:

};

#endif
