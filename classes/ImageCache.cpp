/** ImageCache.cpp
 *  A simple static class that loads SFML images and returns references to them.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm    
*/

#include "ImageCache.h"
#include <dirent.h> // The library that takes care of iterating through files

#include <iostream>
#include <cstring>
using namespace std;

// Initialize static map
map<string, sf::Image> ImageCache::images;


// Load all images in the given directory (recursively)
int ImageCache::LoadFromDirectory ( const char* folder ) {
  
  char filename [256];

  DIR *dir;
  struct dirent *ent;
  dir = opendir ( folder );
  if (dir != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      // Skip the directories
      if ( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 
	   && strcmp(ent->d_name, "jpegs") != 0) {

	strcpy( filename, folder );
	strcat( filename, ent->d_name ); //get full filename relative to game folder
	
	if ( images[ string(ent->d_name) ].LoadFromFile( filename ) ) {
	  cout << "IMAGE CACHE: Loaded image " << ent->d_name << endl;
	} else {
	  cout << "IMAGE CACHE ERROR: Failed to load image " << ent->d_name << endl;
	}

      }
    }
    closedir (dir);

    // No error
    return 0;
    
  } else {
    // Could not open directory, return error
    cout << "IMAGE CACHE ERROR: Could not open directory" << endl;
    return 1;
  }
  
}

// Return image for given name (for example, simple_bit.png)
const sf::Image& ImageCache::GetImage (string name) {

  if( images.find(name) == images.end() )
    cout << "IMAGE CACHE ERROR: Could not find image named " << name << endl;

  return images[name]; // If no match, return an unitialized image to avoid segfaults

}

// C-syle string overload of the previous function
const sf::Image& ImageCache::GetImage (const char* name) { 
  return GetImage( string(name) );
}
