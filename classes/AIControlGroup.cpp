#include "AIControlGroup.h"

using namespace std;

AIControlGroup::AIControlGroup (vector<CellGroup*> c) : ControlGroup(c) {
  selectedUnit = 0;
}
