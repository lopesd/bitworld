/** Animation.cpp 
 *  "Animation" class, responsible for drawing an animated event on screen, like a pulse, or corruption.
 *  Can easily instantiated with a type and a location, and then committed to the level object, 
 *  which should draw it once every frame.
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include "structures.h"

#include <vector>

#include <SFML/Graphics.hpp> 

class Level;

class Animation {
  
 public:
  /** CONSTRUCTOR **/
  Animation ( Location location );

  /** MUTATORS **/
  void addImage         ( std::string name );
  void setImages        ( std::vector<std::string> names );
  void setLocation      ( Location location );
  void setDuration      ( int duration );
  void setSizeInterval  ( float sizeStart, float sizeEnd );
  void setSizeInterval  ( std::vector<float> sizes );
  void setAlphaInterval ( float alphaStart, float alphaEnd );
  void setAlphaInterval ( std::vector<float> alphas );

  /** UTILITY FUNCTIONS **/
  void commit ( Level * level );
  void draw   ( sf::RenderWindow& screen );

  /** ACCESSORS **/
  int isDone ();

 private:
  // SETUP DATA //
  Location   location;        // The grid location of the sprite
  int        duration;        // The duration, in seconds, of the animation
  int        maxFrameCount;   // How many frames the animation takes
  std::vector<float> sizes;   // The sizes to be interpolated between (1 = 1 grid cell size)
  std::vector<float> alphas;  // The alpha values to be interpolated between (0-255)
  std::vector<std::string> imageNames; // Names of the images to be used

  float      cellWidth;       // Taken from the level object, used to calculate sizes
  float      cellHeight;      // Taken from the level object, used to calculate sizes

  // INTERPOLATION DATA //
  int frameCount;    // Tracks the current frame 
  std::vector<int> imageForFrame;   // Images index (in imageNames) to be shown at each frame (interpolated)
  std::vector<float> sizeForFrame;  // Image sizes for each frame (interpolated)
  std::vector<float> alphaForFrame; // Image opacities for each frame (interpolated)

  int done;                   // Indicates if the animation is finished

  sf::Sprite sprite; // SFML Sprite to be drawn

};

#endif
