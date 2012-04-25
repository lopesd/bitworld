#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

class Level;

class Animation {
  
 public:
  Animation( AnimType type, Location location );
  
  void commit( Level * level );

 private:
  Location location;
  AnimType type;

  //sf::Sprite sprite;

};
