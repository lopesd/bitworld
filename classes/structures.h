#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>

typedef struct Direction {int x, y;} Direction;

typedef struct Location  {
  int x, y;
  bool operator< (const Location& loc) const {
    if (this->y < loc.y) return true;
    else if (this->x < loc.x) return true;
    else return false;
  }

  friend std::ostream& operator << (std::ostream& stream, Location self) {
    stream << "(" << self.x << ", " << self.y << ")";
  }

} Location;

#endif
