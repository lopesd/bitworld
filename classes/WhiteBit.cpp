#include "WhiteBit.h"

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" );
  }
}

WhiteBit::WhiteBit (Cell cell) : CellGroup (cell) {}

WhiteBit::~WhiteBit () {}
