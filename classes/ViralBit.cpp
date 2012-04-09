#include "ViralBit.h"

using namespace std;

ViralBit::ViralBit (vector<Cell> c) : CellGroup (c) { 
  weight = 2;
  vector<string> imgs;
  imgs.push_back( "viral_bit.png" );
  imgs.push_back( "viral_bit2.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs );
  }
}

ViralBit::ViralBit (Cell cell) : CellGroup (cell) {}

ViralBit::~ViralBit () {}
