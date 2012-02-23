//Cell.cpp
#include "Cell.h"

Cell::Cell (int c, int r, string img) {
  col = c;
  row = r;
  image = img;
}

Cell::void moveMe (Direction dir) {
  col += dir.x;
  row += dir.y;
  //animate?
}

Cell::void drawMe () {
  //draw!
}
