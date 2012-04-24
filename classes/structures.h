#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>

enum AnimType {NORMAL, PULSER, WHITEBIT};
enum EventType {EMPTY, OPEN, PULSE, CORRUPT};

struct Location;
struct Direction;

typedef struct Direction {
  int x, y;

  bool isZero () {return x == 0 && y == 0;}

  int intabs(int n) const  {
    if(n < 0) n = -n;
    return n;
  }

  int abs() const {return intabs(x)+intabs(y);}

  bool operator< (const Direction& dir)  {
    return ( abs() < dir.abs() );
  }

  bool operator> (const Direction& dir)  {
    return ( abs() > dir.abs() );
  }

  friend std::ostream& operator << (std::ostream& stream, Direction self) {
    stream << "(" << self.x << ", " << self.y << ")";
  }

} Direction;

typedef struct Location {
  int x, y;
  bool operator< (const Location& loc) const {
    if (y < loc.y) return true;
    else if (y > loc.y) return false;
    else return x < loc.x;
  }
  Location operator+ (const Direction& loc) {
    Location temp = {x + loc.x, y + loc.y};
    return temp;
  }
  Location operator- (const Direction& loc) {
    Location temp = {x - loc.x, y - loc.y};
    return temp;
  }
  Location operator+ (const Location& loc) {
    Location temp = {x + loc.x, y + loc.y};
    return temp;
  }
  bool operator== (const Location& loc) {
    return (x == loc.x && y == loc.y);
  }
  bool isZero () {
    return (x == 0 && y == 0);
  }

  friend std::ostream& operator << (std::ostream& stream, Location self) {
    stream << "(" << self.x << ", " << self.y << ")";
  }
} Location;

typedef struct FloatPair {
  float x, y;
} FloatPair;

#endif
