#include "Bit.h"

#include <iostream>
using namespace std;

Bit::Bit (vector<Cell> c) : CellGroup (c) { 
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "simple_bit.png" );
  }
}

Bit::Bit (Cell cell) : CellGroup (cell) {}

Bit::~Bit () {}
