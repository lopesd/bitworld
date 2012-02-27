#include "Cell.h"

Cell::Cell (int c, int r, string img) {
  col = c;
  row = r;
  image = img;
}

void Cell::move (Direction dir) {
  col += dir.x;
  row += dir.y;
  //animate?
}

void Cell::draw () {
  //draw!
}

Location Cell::getGridLocation () {
  Location temp = {col, row};
  return temp;
}

Location Cell::getScreenLocation () {
  Location temp = {x, y};
  return temp;
}
