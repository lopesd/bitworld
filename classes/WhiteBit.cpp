#include "WhiteBit.h"

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  maxResistance = resistance = 5;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" );
  }
}

void WhiteBit::addFlagged( CellGroup* newFlagged ){
  flaggedBits.push_back(newFlagged);
  cout << "Adding a flagged unit to my bits, bitch" << endl;
}
