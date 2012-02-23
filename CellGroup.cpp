//CellGroup.cpp
#include "CellGroup.h"

CellGroup::CellGroup () {
  standardOrderCounter = 0;
}

CellGroup::~CellGroup () {}

CellGroup::void drawMe () {}

//The default neighbor handler -- do nothing
CellGroup::void handleNeighbors (vector<CellGroup*> neighbors) {}

CellGroup::void move () {}

CellGroup::void queueStandardOrders () {}
