#include "Bit.h"

Bit::Bit () {}

Bit::Bit (vector<Cell> cells) : CellGroup (cells) { }

Bit::Bit (Cell cell) : CellGroup (cell) {}
