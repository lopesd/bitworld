#ifndef BYTE_H
#define BYTE_H

#include "CellGroup.h"

class Byte : public CellGroup {

 public:
  Byte ( std::vector<Cell> cells );
  Byte ( const Byte& );

  std::string type () {return std::string("Byte");}

 private:
  void setCellContexts ();

};

#endif
