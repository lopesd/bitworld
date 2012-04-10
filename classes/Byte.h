#ifndef BYTE_H
#define BYTE_H

#include "CellGroup.h"

class Byte : public CellGroup {

 public:
  Byte (std::vector<Cell> cells);

  std::string type ();

 private:

};

#endif
