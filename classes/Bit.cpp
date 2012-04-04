#include "Bit.h"

using namespace std;

Bit::Bit (vector<Cell> c) : CellGroup (c) { 
  weight = 1;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "simple_bit.png" );
  }
}

Bit::Bit (Cell cell) : CellGroup (cell) {}

Bit::~Bit () {}

string Bit::type () {return string("Bit");}
