/*  Structures.h
 *  A series of structs created for easier coding for the game. Includes
 *  Location, Direction, FloatPair, and enumerated types for animations
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>

enum CellStillAnimType   { NORMAL, PULSER, WHITEBIT };
enum CellMoveAnimType    { WALK, PHASE };
enum AnimType            { PULSE, CORRUPT, DETECT, GLOW };

struct Location;
struct Direction;

//used as a 2D math vector to add to locations and in movements
typedef struct Direction {
  int x, y;

  bool isZero () {return x == 0 && y == 0;}

  int intabs(int n) const  { //finds absolute value of a number
    if(n < 0) n = -n;
    return n;
  }

  int abs() const {return intabs(x)+intabs(y);} //returns the sum of the absolute value of x + the abs of y

  bool operator< (const Direction& dir)  { //compares the absolute length of the direction with less than
    return ( abs() < dir.abs() );
  }

  bool operator> (const Direction& dir)  { //compares the absolute length of the direction with greater than
    return ( abs() > dir.abs() );
  }

  friend std::ostream& operator << (std::ostream& stream, Direction self) { //prints out direction in (x,y) format
    stream << "(" << self.x << ", " << self.y << ")";
  }

} Direction;

//A position on the grid
typedef struct Location {
  int x, y;
  bool operator< (const Location& loc) const { //location comparison based off of y value being more important
    if (y < loc.y) return true;
    else if (y > loc.y) return false;
    else return x < loc.x;
  }
  Location operator+ (const Direction& loc) { //allows Locations and directions to be added and return locations 
    Location temp = {x + loc.x, y + loc.y};
    return temp;
  }
  Location operator- (const Direction& loc) { //allows Locations and directions to be subtracted and return locations
    Location temp = {x - loc.x, y - loc.y};
    return temp;
  }
  Location operator+ (const Location& loc) { //locations to add with one another
    Location temp = {x + loc.x, y + loc.y};
    return temp;
  }
  bool operator== (const Location& loc) { //checks if locations are equal by comparing x and ys against each other
    return (x == loc.x && y == loc.y);
  }
  bool isZero () { //returns true if location is 0,0
    return (x == 0 && y == 0);
  }

  friend std::ostream& operator << (std::ostream& stream, Location self) { //prints location in (x,y) format
    stream << "(" << self.x << ", " << self.y << ")";
  }
} Location;

// x and y locations for screen
typedef struct FloatPair {
  float x, y;
} FloatPair;

#endif
