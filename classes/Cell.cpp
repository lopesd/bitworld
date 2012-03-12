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

pair<int, int> Cell::getGridLocation () {
  pair<int, int> temp(col,row);
  return temp;
}

pair<double, double> Cell::getScreenLocation () {
  pair<double, double> temp(x, y);
  return temp;
}
