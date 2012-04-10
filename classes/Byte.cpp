#include "Byte.h"

using namespace std;

Byte::Byte (vector<Cell> c) : CellGroup (c) { 
  weight = 10;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "simple_byte.png" );
  }
}

string Byte::type () {return string("Byte");}
