/** Animation.h
 *  "Animation" class, responsible for drawing an animated event on screen, like a pulse, or corruption.
 *  Can easily instantiated with a type and a location, and then committed to the level object, 
 *  which should draw it once every frame.
 */

#include "Animation.h"

#include "Level.h"
#include "ImageCache.h"

extern int FPS;

using namespace std;

/** CONSTRUCTOR **/

Animation::Animation ( Location l ) {
  location = l;

  done = 0;
  maxFrameCount = 0;
  frameCount = 0;
  sizes.resize( 2, 1 );
  alphas.resize( 2, 255 );
  duration = 1;
}

/** MUTATORS **/

void Animation::addImage ( string name ) {
  imageNames.push_back( name );
}

void Animation::setImages ( vector<string> names ) {
  imageNames = names;
}

void Animation::setLocation ( Location l ) {
  location = l;
}

void Animation::setDuration ( int d ) {
  duration = d;
}

void Animation::setSizeInterval ( float s_start, float s_end ) {
  sizes.clear();
  sizes.push_back( s_start );
  sizes.push_back( s_end );
}

void Animation::setSizeInterval ( vector<float> s ) {
  sizes = s;
}

void Animation::setAlphaInterval ( float a_start, float a_end ) {
  alphas.clear();
  alphas.push_back( a_start );
  alphas.push_back( a_end );
}

void Animation::setAlphaInterval ( std::vector<float> a ) {
  alphas = a;
}

/** UTILITY FUNCTIONS **/

// Initialize myself, and tell level object to take me in
void Animation::commit ( Level* level ) {

  // INITIALIZE THE ANIMATION //

  cellWidth = level->getCellWidth();
  cellHeight = level->getCellHeight();

  maxFrameCount = FPS * duration;
  imageForFrame.resize ( maxFrameCount, -1 );
  sizeForFrame.resize  ( maxFrameCount );
  alphaForFrame.resize ( maxFrameCount );  

  // INTERPOLATION //
  // Interpolate between the images
  if( !imageNames.empty() ) {
    int subsectionSize = maxFrameCount/imageNames.size();
    for( int i = 0; i < imageNames.size(); ++i ) {
      for( int j = 0; j < subsectionSize; ++j ) {
	imageForFrame[i*subsectionSize + j] = i;
      }
    }
  }

  // Interpolate between alpha values
  float alpha = alphas[0];
  int divisions = alphas.size() - 1;
  int subsectionSize = maxFrameCount/divisions;
  for( int i = 0; i < divisions; ++i ) {
    float alphaStep = (alphas[i+1] - alphas[i]) / subsectionSize;
    for( int j = 0; j < subsectionSize; ++j ) {
      alphaForFrame[i*subsectionSize + j] = alpha;
      alpha += alphaStep;
    }
  }

  // Interpolate between size values
  float size = sizes[0];
  divisions = sizes.size() - 1;
  subsectionSize = maxFrameCount/divisions;
  for( int i = 0; i < divisions; ++i ) {
    float sizeStep = (sizes[i+1] - sizes[i]) / subsectionSize;
    for( int j = 0; j < subsectionSize; ++j ) {
      sizeForFrame[i*subsectionSize + j] = size;
      size += sizeStep;
    }
  }

  // SET INITIAL VALUES FOR THE SPRITE'S FIRST DRAW //
  float spriteLocX = location.x*cellWidth  + level->getLeftOffset() + cellWidth/2;
  float spriteLocY = location.y*cellHeight + level->getTopOffset()  + cellHeight/2;
  sprite.SetPosition( spriteLocX, spriteLocY );  
  if( !imageNames.empty() )
    sprite.SetImage( ImageCache::GetImage( imageNames[0] ) );
  sprite.SetCenter( sprite.GetSize() / 2.f );
  sprite.Resize( sizeForFrame[0] * cellWidth, sizeForFrame[0] * cellHeight );
  sprite.SetColor( sf::Color(255, 255, 255, alphaForFrame[0]) );
  

  // COMMIT TO LEVEL //
  level->requestDeafFrames( maxFrameCount );
  level->addAnimation( this );

}

// Draws the sprite and updates the animation data
void Animation::draw ( sf::RenderWindow& screen ) {
  if( done ) return;

  // If the image should be changed since last update
  if( (frameCount > 0)  &&  (imageForFrame[frameCount] != imageForFrame[frameCount-1]) ) {

    //First check for a bad number
    if( imageForFrame[frameCount] >= 0 && imageForFrame[frameCount] < imageNames.size() ) {
      const sf::Image& img = ImageCache::GetImage( imageNames.at( imageForFrame[frameCount]) );
      sprite.SetImage( img );
    } else { //If bad number, make the sprite invisible.
      alphaForFrame[frameCount] = 0;
    }

  }

  // If the size for frame has changed...
  if( (frameCount > 0)  &&  (sizeForFrame[frameCount] != sizeForFrame[frameCount-1]) ) {
    float size = sizeForFrame[frameCount];
    sprite.Resize( size * cellWidth, size * cellHeight );
  }

  // If the alpha has changed...
  if( (frameCount > 0)  &&  (alphaForFrame[frameCount] != alphaForFrame[frameCount-1]) ) {
    sprite.SetColor( sf::Color( 255, 255, 255, alphaForFrame[frameCount]) );
  }

  screen.Draw( sprite );

  if( ++frameCount >= maxFrameCount ) done = 1;
}

int Animation::isDone () {
  return done;
}
