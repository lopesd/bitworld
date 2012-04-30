#include "Wall.h"

using namespace std;

Wall::Wall ( vector<Cell> c ) : CellGroup(c) {
  weight = 100;
  setCellContexts();
}

void Wall::setCellContexts () {

  // Set indicators to the cells around any given cell
  int a, b, l, r, al, ar, bl, br; // above, below, left, right, above-left, etc.
  for(int i = 0; i < cells.size(); ++i ) {
    a=b=l=r=al=ar=bl=br=-1; // Set all directions to 0
    Location myLoc = cells[i].getGridLocation();
    
    for( int j = 0; j < cells.size(); ++j ) {
      Location otherLoc = cells[j].getGridLocation();
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) )
	a = j;
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) )
	b = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )
	r = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )
	l = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) )
	ar = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) ) 
	br = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) ) 
	al = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) ) 
	bl = j;
     
    }
    
    cells[i].setCellContext( a, b, l, r, al, ar, bl, br );
    
    /*
    cout << "For cell "<< i << ", a = " << a << ", b = " << b << ", l = " << l << ", r = " << r
	 << ", ar = " << ar << ", al = " << al << ", br = " << br << ", bl = " << bl << endl;
    */

    // Set the image depending on who is around. We assume the byte is 4 cells big.

    string imgName = "new_wall_";

    // Find the right image and rotation for the cell
    int numConnections = ( int(a!=-1) + int(b!=-1) + int(r!=-1) + int(l!=-1) );
    int rotation = 0;
    if ( numConnections == 0 ) {
      imgName += "0";
    } else if( numConnections == 1 ) {
      imgName += "1";
      if( a != -1 )  rotation = 0;
      if( b != -1 )  rotation = 180;
      if( l != -1 )  rotation = 90;
      if( r != -1 )  rotation = 270;
    } else if ( numConnections == 2 ) {
      imgName += "2";
      if( a != -1  &&  b != -1 ) {
	imgName += "a";
	rotation = 0;
      } else if( r != -1  &&  l != -1 ) {
	imgName += "a";
	rotation = 90;
      } else {
	imgName += "b";
	if( a != -1 && r != -1 ) rotation = 0;
	if( a != -1 && l != -1 ) rotation = 90;
	if( b != -1 && r != -1 ) rotation = 270;
	if( b != -1 && l != -1 ) rotation = 180;
      } 
    } else if ( numConnections == 3 ) {
      imgName += "3";
      if( l == -1 ) rotation = 0;
      if( b == -1 ) rotation = 90;
      if( r == -1 ) rotation = 180;
      if( a == -1 ) rotation = 270;
    } else if ( numConnections == 4 ) {
      imgName += "4";
    }
    
    imgName += ".png";

    cells[i].setImage( imgName );
    cells[i].setSpriteRotation( rotation );
    
  }  
  
}
