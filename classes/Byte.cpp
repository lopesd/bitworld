/** Byte.cpp
 *  The byte is a simple, lumbering, 4 cell big unit that has no further specialization.
 *  Make sure the cells passed to it are correctly placed (in a square formation).
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include "Byte.h"

using namespace std;

/** CONSTRUCTORS **/
Byte::Byte ( vector<Cell> c ) : CellGroup (c) { 
  weight = 10;
  setCellContexts();
}

// Set the images for each cell based on what cells are around it
void Byte::setCellContexts () {

  // Set pointers to every cell to pointers to the cells around it
  int a, b, l, r; // above, below, left, right
  for(int i = 0; i < cells.size(); ++i ) {
    a=b=l=r=-1;
    Location myLoc = cells[i].getGridLocation();

    // Find what cells are around each cell
    for( int j = 0; j < cells.size(); ++j ) {
      Location otherLoc = cells[j].getGridLocation();
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) )  a = j;
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) )  b = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )  r = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )  l = j;
    }
    
    cells[i].setCellContext( a, b, l, r, -1, -1, -1, -1 );
    
    //cout << "For cell "<< i << ", a = " << a << ", b = " << b << ", l = " << l << ", r = " << r << endl;

    // Set the image depending on who is around. We assume the byte is 4 cells big.
    if( cells[i].a != -1 && cells[i].r != -1 ) {
      cells[i].setImage( "new_simple_byte_bl.png" );
    } else if ( cells[i].a != -1 && cells[i].l != -1 ) {
      cells[i].setImage( "new_simple_byte_br.png" );
    } else if ( cells[i].b != -1 && cells[i].r != -1 ) {
      cells[i].setImage( "new_simple_byte_tl.png" );
    } else if ( cells[i].b != -1 && cells[i].l != -1 ) {
      cells[i].setImage( "new_simple_byte_tr.png" );
    }
    
  }  

}

//Byte can't be corrupted
void Byte::dropResistance ( int n ){}
