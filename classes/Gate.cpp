/** Gate.cpp
 *  A gate does not derive from CellGroup and is not a "unit" per se.
 *  It is responsible for drawing its cells, and is the base class for other specialized gates.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Gate.h"

#include "Animation.h"

#include <cstring>

using namespace std;

/** CONSTRUCTORS **/
Gate::Gate ( vector<Cell> c ) {
  cells = c;
  weight = 3; // The standard weight of a gate is 3
  openCounter = weight;
  locked = 0;
  setCellContexts();
}

/** UTILITY FUNCTIONS **/
void Gate::setCellContexts () {
  // Set indicators to the cells around any given cell
  int a, b, l, r, al, ar, bl, br; // above, below, left, right, above-left, etc.
  for(int i = 0; i < cells.size(); ++i ) {
    a=b=l=r=al=ar=bl=br=-1; // Set all directions to 0
    Location myLoc = cells[i].getGridLocation();
    
    for( int j = 0; j < cells.size(); ++j ) {
      Location otherLoc = cells[j].getGridLocation();
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) )            a = j;
      if( (myLoc.x == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) )	        b = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )    	r = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == otherLoc.y) )    	l = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) )	ar = j;
      if( ((myLoc.x + 1) == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) ) 	br = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == (otherLoc.y + 1)) ) 	al = j;
      if( ((myLoc.x - 1) == otherLoc.x) && (myLoc.y == (otherLoc.y - 1)) ) 	bl = j;
    }
    
    cells[i].setCellContext( a, b, l, r, al, ar, bl, br );  
  }

  setCellImages(); //The contexts have been updated -- set the images

}

void Gate::setCellImages () {

  for( int i = 0; i < cells.size(); ++i ) {
    // Set the image of each piece of the gate depending on who is around.
    string imgName;
    if( locked )
      imgName = "closed_gate_";
    else
      imgName = "open_gate_";
    
    // Find the right image and rotation for the cell
    int a = cells[i].a;
    int b = cells[i].b;
    int l = cells[i].l;
    int r = cells[i].r;
    int numConnections = ( int(a!=-1) + int(b!=-1) 
			   + int(r!=-1) + int(l!=-1) );
    int rotation = 0;  // how to rotate the sprite
    int horizFlip = 0; // indicates if the sprite should be flipped
  
    if ( numConnections == 0 ) {
      imgName += "0";
    } else if( numConnections == 1 ) {
    
      imgName += "1";
      if( a != -1 )  rotation = 0;
      if( b != -1 )  {rotation = 180; horizFlip = 1;}
      if( l != -1 )  rotation = 90;
      if( r != -1 )  {rotation = 270; horizFlip = 1;}
    
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
	if( a != -1 && r != -1 ) {rotation = 0; imgName += "l";}
	if( a != -1 && l != -1 ) {rotation = 0; horizFlip = 1; imgName += "l";}
	if( b != -1 && r != -1 ) {rotation = 0; horizFlip = 1; imgName += "r";}
	if( b != -1 && l != -1 ) {rotation = 0; imgName += "r";}
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
  
    // imgName should now be a string indicating the correct image
  
    cells[i].setImage( imgName );
    cells[i].setSpriteRotation( rotation );
    if(horizFlip) cells[i].flipSprite();
  }    
  
}

// Draw the gate on the given sfml window
void Gate::draw ( sf::RenderWindow &screen ) {
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].draw( screen );
  }
}

// Perform the unit's high cycle, checks for opening
void Gate::highCycle () {

  // Only check for units on me if I am not locked
  if( !locked ) {

    unitsToTransfer.clear();
    int foundUnit = 0;
    //int prevOpenCounter = openCounter; // Used for animation
    for( int i = 0; i < cells.size(); ++i ) { 
      CellGroup* unitOnMe = level->unitAtLocation( cells[i].getGridLocation() );
      if( unitOnMe ) { // There is a unit on me
	if( strcmp(unitOnMe->CGGroupName.c_str(), "user") == 0 ) {
	  unitsToTransfer.insert( unitOnMe );
	  foundUnit = 1;
	  --openCounter;
	}
      }
    }

    if( !foundUnit ) {
      openCounter = weight;
    } else { // There is someone attempting to open us -- create animation
      Animation anim( getLocations()[0] );
      anim.fromCountDownPreset( openCounter, openCounter );
      anim.commit( level );
    }

    if( openCounter <= 0 ) { //Gate opens
      level->openGate( this );
    }

  } // end locked if statement

}

/** MUTATORS **/
void Gate::setLocked ( int l ) {
  locked = l;
  cout << "Setting locked to " << l << endl;
  setCellImages(); //Update the images of a locked gate
}

void Gate::setGridData ( int w, int h, int t, int l ) {
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setGridData( w, h, t, l );
  }
}

void Gate::setWeight ( int w ) {
  weight = w;
  openCounter = weight;
}

void Gate::setDest ( std::string s ) {
  dest = s;
}

void Gate::resetOpenCounter () {
  openCounter = weight;
}

/** ACCESSORS **/
int Gate::getLocked () {
  return locked;
}

string Gate::destination () {
  return dest;
}

set<CellGroup*> Gate::getUnitsToTransfer () {
  return unitsToTransfer;
}

vector<Location> Gate::getLocations () {
  vector<Location> locs;
  for( int i = 0; i < cells.size(); ++i )
    locs.push_back( cells[i].getGridLocation() );
  return locs;
}
