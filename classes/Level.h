#ifndef LEVEL_H
#define LEVEL_H

#include "ControlGroup.h"
#include "CellGroup.h"
#include "structures.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <map>


class Level {
 public:
  Level (sf::RenderWindow &window, vector<ControlGroup*>, vector<CellGroup*> units,
         int width=13, int height=9, int cpp=3);

  void display     ();
  void prepareInput(int x, int y);
  void handleInput (Location  loc);
  void handleInput (Direction dir);
  void run         ();
  vector<CellGroup*> findNeighbors (CellGroup*);
  void drawGrid    ();
  void drawUnits   ();
  void highlightSelect();
 private:
  ControlGroup*                activeGroup;
  vector<ControlGroup*>        controlGroups;
  vector<CellGroup*>           units;
  map<Location, CellGroup*>    grid;
  int width, height;
  int cyclesPerPeriod;

  int gridRowHeight;
  int gridColWidth;

  sf::RenderWindow& window;

  void updateGrid();
};

#endif
