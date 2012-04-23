/** Cell.cpp
 *  The cell class defines a single cell on the grid.
 *  It is responsible for drawing and moving itself, and contains information on its location.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Cell.h"

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

const extern int FPS;

// Constructor takes location on grid as input
Cell::Cell (int c, int r) {
  col = c;
  row = r;
  x = 0;
  y = 0;
  animIncrement.x = 0; animIncrement.y = 0;
  moveCount = 0;
  framesToMove = FPS/2;
  stillAnimCount = imageIndex = 0;
  stillAnimType = NORMAL;
}

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
  
  setGridData( c.width, c.height, c.top_offset, c.left_offset );
  // If Cell being copied had an image
  if ( !c.imageNames.empty() )
    setImages( c.imageNames ); // Set new Cell to have the same one
}

// Move in the given direction. Updates col and row data
void Cell::move ( Direction dir ) {
  col += dir.x;
  row += dir.y;

  if( movementAnimType == WHITEBIT ) {
    if( dir.isZero() ) return;
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
  sprite.Resize( width, height );
  if( sprite.GetImage() )
    sprite.SetCenter( sprite.GetImage()->GetWidth() / 2, sprite.GetImage()->GetHeight() / 2 );
  sprite2.SetPosition( x, y );
  sprite2.Resize( width, height );
  if( sprite2.GetImage() )
    sprite2.SetCenter( sprite2.GetImage()->GetWidth() / 2, sprite2.GetImage()->GetHeight() /2 );
}

// Update image. Image must be loaded in the ImageCache beforehand
void Cell::setImage ( string name ) {
  imageNames.push_back( name );
  sprite.SetImage( ImageCache::GetImage(name) );
}

// C-style string overload of above function
void Cell::setImage ( const char* name ) {
  setImage( string(name) );
}

// Set images for the cell's animation
void Cell::setImages ( vector<string> imgs, enum AnimType aType ) {
  imageNames = imgs;
  stillAnimType = aType;
  if( stillAnimType == PULSER ) stillAnimCount = 1;
  else                     stillAnimCount = 0;

  if( !imgs.empty() ) {
    sprite.SetImage( ImageCache::GetImage(imageNames[0]) );
    if( stillAnimType == PULSER ) {
      sprite2.SetImage( ImageCache::GetImage(imageNames[1]) );
    }
  }
  else cout << "ERROR >> Image vector passed is empty." << endl;
}

// Set the movement animation type
void Cell::setMovementAnimation( AnimType type ) {
  movementAnimType = type;
  if( type == WHITEBIT ) framesToMove = FPS;
}

// Draw Cell on the given SFML screen object
void Cell::draw ( sf::RenderWindow& screen ) {

  //MOVEMENT ANIMATION
  if( moveCount != 0 ) {
    --moveCount;
    
    if( movementAnimType == WHITEBIT ) {

      if( moveCount == framesToMove/2 ) { //move the sprite at the halfway point
	x = (left_offset+col*width + 0.5*width);
	y = (top_offset+row*height + 0.5*height);
	sprite.SetPosition( x, y );
      }
      
      float alpha = fadeIncrement*abs(moveCount-framesToMove/2);
      sprite.SetColor( sf::Color(255, 255, 255, alpha) );

    } else {
      x += animIncrement.x;
      y += animIncrement.y;
      sprite.Move( animIncrement.x, animIncrement.y );
      sprite2.Move( animIncrement.x, animIncrement.y );
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

    if( ++stillAnimCount > 15 ) {
      stillAnimCount = 0;
      if( ++imageIndex >= imageNames.size() ) imageIndex = 0;
      sprite.SetImage( ImageCache::GetImage(imageNames[imageIndex]) );
    }
    
    screen.Draw( sprite );
    
  }

  else if( stillAnimType == PULSER ) {
    static float rotation = 0;
    screen.Draw( sprite );
    sprite2.SetRotation( rotation++ );
    screen.Draw( sprite2 );
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
