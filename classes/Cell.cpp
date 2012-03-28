/** Cell.cpp
 *  The cell class defines a single cell on the grid.
 *  It is responsible for drawing and moving itself, and contains information on its location.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Cell.h"

#include <iostream>
#include <cstring>

using namespace std;

// Constructor takes location on grid as input
Cell::Cell (int c, int r) {
  col = c;
  row = r;
  x = 0;
  y = 0;
  imageName = "";
}

// Copy constructor is defined to make sure the sprite information is copied
Cell::Cell ( const Cell& c ) {
  col = c.col;
  row = c.row;
  x = c.x;
  y = c.y;
  width = c.width; height = c.height; top_offset = c.top_offset; left_offset = c.left_offset;
  
  // If Cell being copied had an image
  if ( strcmp(c.imageName.c_str(), "") != 0 )
    setImage( c.imageName ); // Set new Cell to have the same one
  else
    imageName = "";
}

// Move in the given direction. Updates col, row, x and y data.
void Cell::move ( Direction dir ) {
  col += dir.x;
  row += dir.y;
  x   += dir.x*width;
  y   += dir.y*height;
  sprite.Move( width*dir.x, height*dir.y );
  //animate?
}

// Privately store the measurements of the grid. Should be called when Cell is passed to a Level object
void Cell::setGridData ( int w, int h, int t, int l ) {
  width = w;
  height = h;
  top_offset = t;
  left_offset = l;
  x = left_offset + width*col;
  y = top_offset + height*row;
  
  sprite.SetPosition(  x, y );
  sprite.Resize( width, height );
}

// Update image. Image must be loaded in the ImageCache beforehand
void Cell::setImage ( string name ) {
  imageName = name;
  sprite.SetImage( ImageCache::GetImage(imageName) );
}

// C-style string overload of above function
void Cell::setImage ( const char* name ) {
  setImage( string(name) );
}

// Draw Cell on the given SFML screen object
void Cell::draw ( sf::RenderWindow& screen ) {
  screen.Draw( sprite );
}

// Return location on the grid
Location Cell::getGridLocation () {
  Location temp = {col, row};
  return temp;
}

// Return location on the screen (pixels)
Location Cell::getScreenLocation () {
  Location temp = {x, y};
  return temp;
}

