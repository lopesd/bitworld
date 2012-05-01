/** Animation.h
 *  "Animation" class, responsible for drawing an animated event on screen, like a pulse, or corruption.
 *  Can easily instantiated with a type and a location, and then committed to the level object, 
 *  which should draw it once every frame.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
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
  rotations.resize( 2, 0 );
  duration = 1;
}

/** MUTATORS **/

void Animation::addImage ( string name ) {
  imageNames.push_back( name );
}

void Animation::setImages ( vector<string> names ) {
  imageNames = names;
}

void Animation::setColor ( sf::Color cor ) {
  sprite.SetColor( cor );
}

void Animation::setLocation ( Location l ) {
  location = l;
}

void Animation::setDuration ( float d ) {
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

void Animation::setAlphaInterval ( vector<float> a ) {
  alphas = a;
}

void Animation::setRotationInterval ( float r_start, float r_end ) {
  rotations.clear();
  rotations.push_back( r_start );
  rotations.push_back( r_end );
}

void Animation::setRotationInterval ( vector<float> r ) {
  rotations = r;
}

// Preset counter type animation. Interpolates between the given numbers
void Animation::fromCountDownPreset ( int begin, int end ) {
  vector<int> nums;
  int increment = begin > end ? -1 : 1 ;
  for( ; begin != end; begin+=increment ) {
    nums.push_back( begin );
  }
  nums.push_back( begin );
  fromCountDownPreset( nums );
}

// Preset counter type animation. Interpolates between the given numbers
void Animation::fromCountDownPreset ( vector<int> nums ) {
  imageNames.clear();
  // Places images sequentially in the imgNames vector
  for( int i = 0; i < nums.size(); ++i ) {
    if( nums[i] == 1 ) imageNames.push_back( "one.png" );
    else if( nums[i] == 2 ) imageNames.push_back( "two.png" );
    else if( nums[i] == 3 ) imageNames.push_back( "three.png" );
    else if( nums[i] == 4 ) imageNames.push_back( "four.png" );
    else if( nums[i] == 0 ) imageNames.push_back( "zero.png" );
  }
}

/** UTILITY FUNCTIONS **/

// Fills given vector with an interpolation of the values in the other vector
void Animation::interpolate( vector<float>& values, vector<float>& results ) {

  float value = values[0];
  int divisions = values.size() - 1;
  int subsectionSize = results.size()/divisions;
  int i, j;
  for( i = 0; i < divisions; ++i ) { // Interpolate!
    float valueStep = (values[i+1] - values[i]) / subsectionSize;
    for( j = 0; j < subsectionSize; ++j ) {
      results[i*subsectionSize + j] = value;
      value += valueStep;
    }
  }
  
  //If the division is uneven, fill the last remaining ones ones with the final value
  if( (--i*subsectionSize + --j ) < results.size()-1 ) {
    for( ; i*subsectionSize+j < results.size(); ++j ) {
      results[i*subsectionSize + j] = value;
    }
  }
  
}

// Initialize the drawing to be drawn on the given level
void Animation::init ( Level* level ) {

  // Check for location validity...
  if( location.x < 0 || location.y < 0 || location.x >= level->getWidth() || location.y >= level->getHeight() ) 
    return;

  // INITIALIZE THE ANIMATION //

  cellWidth = level->getCellWidth();
  cellHeight = level->getCellHeight();

  maxFrameCount = (float)FPS * duration;

  // Clear and resize the vectors to be interpolated into
  imageForFrame.clear();
  sizeForFrame.clear();
  alphaForFrame.clear();
  rotationForFrame.clear();
  imageForFrame.resize ( maxFrameCount, -1 );
  sizeForFrame.resize  ( maxFrameCount, 1 );
  alphaForFrame.resize ( maxFrameCount, 255 );  
  rotationForFrame.resize ( maxFrameCount, 0 );

  // INTERPOLATION //
  // Split the imageForFrame up evenly among the imageNames
  if( !imageNames.empty() ) {
    int subsectionSize = maxFrameCount/imageNames.size();
    for( int i = 0; i < imageNames.size(); ++i ) {
      for( int j = 0; j < subsectionSize; ++j ) {
	imageForFrame[i*subsectionSize + j] = i;
      }
    }
  }

  interpolate( alphas, alphaForFrame );
  interpolate( sizes, sizeForFrame );
  interpolate( rotations, rotationForFrame );

  // SET INITIAL VALUES FOR THE SPRITE'S FIRST DRAW //
  // Set initial position
  float spriteLocX = location.x*cellWidth  + level->getLeftOffset() + cellWidth/2;
  float spriteLocY = location.y*cellHeight + level->getTopOffset()  + cellHeight/2;
  sprite.SetPosition( spriteLocX, spriteLocY );  
  // Set initial image and center
  if( !imageNames.empty() )
    sprite.SetImage( ImageCache::GetImage( imageNames[0] ) );
  sprite.SetCenter( sprite.GetSize() / 2.f );
  // Set initial size
  sprite.Resize( sizeForFrame[0] * cellWidth, sizeForFrame[0] * cellHeight );
  // Set initial alpha
  sprite.SetColor( sf::Color(255, 255, 255, alphaForFrame[0]) );
  // Set inital rotation
  sprite.SetRotation( rotations[0] );

}

// Initialize myself, and tell level object to take me in
void Animation::commit ( Level* level ) {

  // INITIALIZE //
  init( level );
  
  // COMMIT TO LEVEL //
  level->requestDeafFrames( maxFrameCount );
  level->addAnimation( this );

}

// Draws the sprite and updates the animation data
void Animation::draw ( sf::RenderWindow& screen ) {
  if( done ) return;

  // Check for any necessary updates
  if( frameCount > 0 ) {

    if( imageForFrame[frameCount] != imageForFrame[frameCount-1] ) {

      //First check for a bad number
      if( imageForFrame[frameCount] >= 0 && imageForFrame[frameCount] < imageNames.size() ) {
	const sf::Image& img = ImageCache::GetImage( imageNames.at( imageForFrame[frameCount]) );
	sprite.SetImage( img );
      } else { //If bad number, make the sprite invisible.
	alphaForFrame[frameCount] = 0;
      }

    }

    // If the size for frame has changed...
    if( sizeForFrame[frameCount] != sizeForFrame[frameCount-1] )
      sprite.Resize( sizeForFrame[frameCount] * cellWidth, sizeForFrame[frameCount] * cellHeight );

    // If the alpha has changed...
    if( alphaForFrame[frameCount] != alphaForFrame[frameCount-1] )
      sprite.SetColor( sf::Color( 255, 255, 255, alphaForFrame[frameCount]) );

    // If the rotation has changed...
    if( rotationForFrame[frameCount] != rotationForFrame[frameCount-1] )
      sprite.SetRotation( rotationForFrame[frameCount] );

  }

  screen.Draw( sprite );

  if( ++frameCount >= maxFrameCount ) done = 1;
}

/** ACCESSORS **/

int Animation::isDone () {
  return done;
}
