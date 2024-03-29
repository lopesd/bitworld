/** Bit.cpp
 *  The simplest of Bits, this bit does nothing but follow given orders.
 *  It is hardly specialized any more than CellGroup.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "Bit.h"

using namespace std;

/** CONSTRUCTORS **/
Bit::Bit (vector<Cell> c) : CellGroup (c) { 
  weight = 1;
  // Set images
  vector<string> imgs;
  imgs.push_back( "new_simple_bit.png" );
  imgs.push_back( "new_simple_bit2.png" );
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImages( imgs );
  }
}
