#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"

class WhiteBit : public CellGroup {

 public:
  WhiteBit (std::vector<Cell> cells);
  WhiteBit (Cell cell);
  WhiteBit ();
  ~WhiteBit();

 private:

};

#endif
