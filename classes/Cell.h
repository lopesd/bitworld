#ifndef CELL_H
#define CELL_H

#include "structures.h"
#include <string>

using namespace std;

class Cell {

 public:
  Cell (int col = -1, int row = -1, string img = "");
  void move (Direction dir);
  void draw ();

  Location getGridLocation ();
  Location getScreenLocation ();

 private:
  int col, row;
  double x, y;
  string image;

};

#endif
