#include "WhiteBit.h"

#include <set>

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  maxResistance = resistance = 5;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" ); 
    cells[i].setMovementAnimation( WHITEBIT );
  }
}

void WhiteBit::findClosest () {};

Direction WhiteBit::findMove( map<Location, CellGroup*> grid ) {
  Direction dir = {0,0};
  if( !controlGroup->level->getFlaggedUnits().empty() ) {
    dir.x = 1;
  }
  return dir;
}

/*
void WhiteBit::getMovement( int n ) {
  Direction dir;

  if( controlGroup->level->getFlaggedUnits.empty() ) {
    dir.x = 0; dix.y = 0;
  } else {
    findClosest( controlGroup->level->getFlaggedUnits() );
    dir = findMove();
  }

  return dir;
}
*/
