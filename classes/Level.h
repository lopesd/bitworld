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
  Level (sf::RenderWindow &window);
  Level (const Level&);
  
  void init (vector<ControlGroup*>, vector<CellGroup*> units, int width=13, int height=9, int cpp=3);

  void display     ();
  void prepareInput(int x, int y);
  void handleInput (Location  loc);
  void handleInput (Direction dir);
  void handleInput (sf::Key::Code);
  void run         ();
  vector<CellGroup*> findNeighbors (CellGroup*);
  void drawGrid    ();
  void drawUnits   ();
  void highlightSelect();

  void drawArrows  ();
  void drawBackground();
  void drawUpArrow(int);
  void drawDownArrow(int);
  void drawLeftArrow(int);
  void drawRightArrow(int);

 private:
  ControlGroup*                activeGroup;
  vector<ControlGroup*>        controlGroups;
  vector<CellGroup*>           units;
  map<Location, CellGroup*>    grid;
  int width, height;
  int cyclesPerPeriod;

  int gridRowHeight;
  int gridColWidth;

  void updateGrid();

  sf::RenderWindow& window;
};

#endif
