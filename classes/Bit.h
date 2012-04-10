#ifndef BIT_H
#define BIT_H

#include "CellGroup.h"

class Bit : public CellGroup {

 public:
  Bit (std::vector<Cell> cells);

  std::string type ();

 private:

};

#endif
