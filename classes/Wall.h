#ifndef WALL_H
#define WALL_H

#include "CellGroup.h"

class Wall : public CellGroup {
  
 public:
  Wall( std::vector<Cell> cells );
  Wall( const Wall& );

  void dropResistance ( int n ) {} //Override so that the wall doesn't get corrupted

  std::string type () {return std::string("Wall");}

 private:
  void setCellContexts ();
  
};

#endif
