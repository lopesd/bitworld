#include "ViralBit.h"

using namespace std;

ViralBit::ViralBit (vector<Cell> c) : CellGroup (c) { 
  weight = 2;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "viral_bit.png" );
  }
}

ViralBit::ViralBit (Cell cell) : CellGroup (cell) {}

ViralBit::~ViralBit () {}
