/** Cell.h
 *  The cell class defines a single cell on the grid.
 *  It is responsible for drawing and moving itself, and contains information on its location.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>

#include "structures.h"
#include "ImageCache.h"
#include <string>

class Cell {
  
 public:
  // Constructor takes location on grid as input
  Cell ( int col = -1, int row = -1 );
  // Copy constructor is defined to make sure the sprite information is copied
  Cell ( const Cell& );
  
  // Move in the given direction. Updates col, row, x and y data.
  void move ( Direction dir );
  // Draw on the given SFML screen object
  void draw ( sf::RenderWindow& screen );
  
  // Privately store the measurements of the grid. Should be called when Cell is passed to a Level object
  void setGridData ( int width, int height, int top_offset, int left_offset );
  // Update image. Image must be loaded in the ImageCache beforehand
  void setImage  ( std::string imageName );
  void setImage  ( const char* imageName );
  void setImages ( std::vector<std::string>, AnimType = NORMAL );
  void setMovementAnimation( AnimType type );

  // Return location on the grid or location on the screen (pixels)y
  Location getGridLocation ();
  FloatPair getScreenLocation ();
  int getMoveCount ();
  
 private:
  // Location information
  int col, row;
  double x, y;

  // Grid information, stored locally, used for drawing
  float width, height, top_offset, left_offset;

  // Image and animation stuff
  int framesToMove; //indicates the amount of frames it takes to run the movement animation
  FloatPair animIncrement;
  float fadeIncrement; //used for white bits' fading animation
  int moveCount;
  AnimType stillAnimType, movementAnimType; //types of animation
  int stillAnimCount, imageIndex;
  std::vector<std::string> imageNames;
  sf::Sprite sprite;
  sf::Sprite sprite2;

};

#endif
