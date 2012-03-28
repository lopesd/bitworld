/** ImageCache.h
 *  A simple static class that loads SFML images and returns references to them.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <SFML/Graphics.hpp>

#include <map>

class ImageCache {
  
 public:
  // Load all images in the given directory (recursively)
  static int LoadFromDirectory( const char* folder );

  // Return image for given name (for example, simple_bit.png)
  static const sf::Image& GetImage (const char* imgname);
  static const sf::Image& GetImage (std::string imgname);

 private:
  // Private constructors do not allow this class to be instantiated
  ImageCache  ();
  ~ImageCache ();

  // The images, mapped to their names
  static std::map<std::string, sf::Image> images;
  
};

#endif
