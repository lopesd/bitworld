#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

#include <SFML/Graphics.hpp>

#include "Level.h"
#include "CellGroup.h"
#include "structures.h"
#include <vector>

class Level; //Forward declaration
class CellGroup; 

class ControlGroup {

 public:
  ControlGroup (std::vector<CellGroup*>);

  virtual void startTurn   () = 0;
  virtual void handleInput (Direction dir) = 0;
  virtual void handleInput (CellGroup* unit) = 0;
  virtual void handleInput (sf::Key::Code) = 0;

  void killCell (CellGroup* cell);

  //ACCESSORS
  CellGroup* getSelectedUnit();

  //MUTATORS
  void setLevel (Level* level);
  void clearSelection ();

  Level* level;

 protected:
  std::vector<CellGroup*> units;
  CellGroup* selectedUnit;

};

#endif
