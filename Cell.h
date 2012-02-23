// Cell.h
#ifndef CELL_H
#define CELL_H

#include "structure"

using namespace std;

class Cell {

 public:
  Cell (int col, int row, string img = "");
  void moveMe (Direction dir);
  void drawMe ();

 private:
  int col, row;
//  double x, y;
  string image;

};

#endif
