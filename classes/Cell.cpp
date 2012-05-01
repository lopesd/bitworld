/** Cell.cpp
 *  The cell class defines a single cell on the grid.
 *  It is responsible for drawing and moving itself, and contains information on its location.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <cmath>

#define PI 3.14159265

using namespace std;

const extern int FPS;

/** CONSTRUCTORS **/

Cell::Cell (int c, int r) {
  col = c;
  row = r;
  x = 0;
  y = 0;
  animIncrement.x = 0; animIncrement.y = 0;
  moveCount = 0;
  framesToMove = FPS/3;
  alpha = 255;
  stillAnimCount = imageIndex = 0;
  stillAnimType = NORMAL;
  sinCounter = rotation = 0;
  a=b=l=r=ar=al=br=bl= -1;
}

/*
// Copy constructor is defined to make sure the sprite information is copied
Cell::Cell ( const Cell& c ) {
  col = c.col;
  row = c.row;
  stillAnimCount = c.stillAnimCount;
  imageIndex = c.imageIndex;
  stillAnimType = c.stillAnimType;
  animIncrement = c.animIncrement;
  moveCount = c.moveCount;
  framesToMove = c.framesToMove;
  alpha = c.alpha;
  sinCounter = rotation = 0;
  a=b=l=r=ar=al=br=bl= -1;

  setGridData( c.width, c.height, c.top_offset, c.left_offset );
  // If Cell being copied had an image
  if ( !c.imageNames.empty() )
    setImages( c.imageNames ); // Set new Cell to have the same one
}
*/

/** UTILITY FUNCTIONS **/
// Move in the given direction. Updates col and row data
void Cell::move ( Direction dir ) {
  col += dir.x;
  row += dir.y;

  if( movementAnimType == PHASE ) {
    moveCount = framesToMove;
    fadeIncrement = (float)255/moveCount*2;
  } else {
    moveCount = framesToMove; // A larger number means slower movement
    animIncrement.x = ((left_offset+col*width + 0.5*width)  - x ) / moveCount;
    animIncrement.y = ((top_offset+row*height + 0.5*height) - y ) / moveCount;
    if( abs(animIncrement.x) < 0.01 ) animIncrement.x = 0;
    if( abs(animIncrement.y) < 0.01 ) animIncrement.y = 0;
  }

}

// Privately store the measurements of the grid. Should be called when Cell is passed to a Level object
void Cell::setGridData ( int w, int h, int t, int l ) {
  width = w;
  height = h;
  top_offset = t;
  left_offset = l;
  x = left_offset + width*col + 0.5*width;
  y = top_offset + height*row + 0.5*height;
  
  sprite.SetPosition( x, y );
  sprite2.SetPosition( x, y );
  sprite.Resize( width, height );
  if( stillAnimType != WHITEBIT ) // don't resize if it's a white bit halo
    sprite2.Resize( width, height );  
}

// Set relative location (used for large units) 
void Cell::setCellContext( int a_, int b_, int l_, int r_, int ar_, int al_, int br_, int bl_ ) {
  a = a_;
  b = b_;
  l = l_;
  r = r_;
  ar = ar_;
  al = al_;
  br = br_;
  bl = bl_;
}

// Update image. Image must be loaded in the ImageCache beforehand
void Cell::setImage ( string name ) {
  imageNames.push_back( name );
  sprite.SetImage( ImageCache::GetImage(name) );
  sprite.SetCenter( sprite.GetSize() / 2.f );
}

// C-style string overload of above function
void Cell::setImage ( const char* name ) {
  setImage( string(name) );
}

// Set images for the cell's animation
void Cell::setImages ( vector<string> imgs, enum CellStillAnimType aType ) {
  imageNames = imgs;
  stillAnimType = aType;
  if( stillAnimType == PULSER ) stillAnimCount = 1;
  else                          stillAnimCount = 0;

  if( !imgs.empty() ) {
    sprite.SetImage( ImageCache::GetImage(imageNames[0]) );
    sprite.SetCenter( sprite.GetSize() / 2.f );
    if( stillAnimType == PULSER ) {
      sprite2.SetImage( ImageCache::GetImage(imageNames[1]) );
      sprite2.SetCenter( sprite2.GetSize() / 2.f );
    }
    if( stillAnimType == WHITEBIT ) {
      sprite2.SetImage( ImageCache::GetImage(imageNames[1]) );
      sprite2.SetCenter( sprite2.GetSize() / 2.f );
    }
  }
  else cout << "ERROR >> Image vector passed is empty." << endl;
}

// Set the rotation for the sprite
void Cell::setSpriteRotation ( float r ) {
  sprite.SetRotation( r );
  sprite2.SetRotation( r );
}

// Flip the sprite horizontally 
void Cell::flipSprite () {
  sprite.FlipX( 1 );
}

// Set the movement animation type
void Cell::setMovementAnimation( CellMoveAnimType type ) {
  movementAnimType = type;
  if( type == PHASE ) framesToMove = FPS;
}

// Move to a place on the grid
void Cell::setGridLocation ( Location newLoc ) {
  col = newLoc.x;
  row = newLoc.y;
}

// Draw Cell on the given SFML screen object
void Cell::draw ( sf::RenderWindow& screen ) {

  //MOVEMENT ANIMATION
  if( moveCount != 0 ) {
    --moveCount;

    // PHASING ANIMATION
    if( movementAnimType == PHASE ) {

      if( moveCount == framesToMove/2 ) { //move the sprite at the halfway point
	x = (left_offset+col*width + 0.5*width);
	y = (top_offset+row*height + 0.5*height);
	sprite.SetPosition( x, y );
	sprite2.SetPosition( x, y );
      }
      
      // Increment the unit's transparency.
      alpha = fadeIncrement*abs(moveCount-framesToMove/2);
      sprite.SetColor( sf::Color(255, 255, 255, alpha) );

    } else {
      // NORMAL MOVEMENT ANIMATION
      x += animIncrement.x;
      y += animIncrement.y;
      sprite.Move( animIncrement.x, animIncrement.y );
      sprite2.Move( animIncrement.x, animIncrement.y );

      // Finalize the movement.
      if( moveCount == 0 ) {
	x = (left_offset+col*width + 0.5*width);
	y = (top_offset+row*height + 0.5*height);
	animIncrement.x = 0;
	animIncrement.y = 0;
      }
    }
    
  }
  
  // IN PLACE ANIMATION
  if( stillAnimType == NORMAL ) {
    // NORMAL STILL ANIMATION

    // Switch linearly between the images given
    if( ++stillAnimCount > 15 ) {
      stillAnimCount = 0;
      if( ++imageIndex >= imageNames.size() ) imageIndex = 0;
      sprite.SetImage( ImageCache::GetImage(imageNames[imageIndex]) );
    }
    
    screen.Draw( sprite );
    
  }

  else if( stillAnimType == PULSER ) {
    // PULSER STILL ANIMATION
    screen.Draw( sprite );
    sprite2.SetRotation( rotation++ ); // Rotate middle thingy
    screen.Draw( sprite2 );
  }
  
  else if( stillAnimType == WHITEBIT ) {
    // WHITE BIT STILL ANIMATION
    sprite2.SetColor( sf::Color( 255, 255, 255, abs(sin((sinCounter+=2)*PI/180)*255)*(alpha/255) ) ); // Glow
    if( sinCounter >= 360 ) sinCounter = 0;
    screen.Draw( sprite2 );
    screen.Draw( sprite );
  }

}

// Return location on the grid
Location Cell::getGridLocation () {
  Location temp = {col, row};
  return temp;
}

// Return location on the screen (pixels)
FloatPair Cell::getScreenLocation () {
  FloatPair temp = {x, y};
  return temp;
}

// Return moveCount (how many frames the cell still needs to complete its animation)
int Cell::getMoveCount () {
  return moveCount;
}
