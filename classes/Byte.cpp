#include "Byte.h"

using namespace std;

Byte::Byte ( vector<Cell> c ) : CellGroup (c) { 
  weight = 10;
  setCellContexts();
}

void Byte::setCellContexts () {

  // Set pointers to every cell to pointers to the cells around it
  int a, b, l, r; // above, below, left, right
  for(int i = 0; i < cells.size(); ++i ) {
    a=b=l=r=-1;
    Location myLoc = cells[i].getGridLocation();
    
    for( int j = 0; j < cells.size(); ++j ) {
      Location otherLoc = cells[j].getGridLocation();
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) ) {
	cout << "a detected  ";
	a = j;
      }
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) ) {
	cout << "b detected  ";
	b = j;
      }
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == otherLoc.y) ) {
	cout << "r detected  ";
	r = j;
      }
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == otherLoc.y) ) {
	cout << "l detected  ";
	l = j;
      }
      cout << endl;
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
