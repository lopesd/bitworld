/** Level.h
 *  The level object contains all the information about the grid for a certain level.
 *  It receives and interprets input from the user and controls ControlGroups.
 *  It is also responsible for drawing the grid portion of the screen.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "ControlGroup.h"
#include "CellGroup.h"
#include "ImageCache.h"
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
  void prepareInput(int x, int y, int isRightClick);
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

 private:
  ControlGroup*                activeGroup;
  vector<ControlGroup*>        controlGroups;
  vector<CellGroup*>           units;
  map<Location, CellGroup*>    grid;
  int width, height;
  int cyclesPerPeriod;

  int top_offset, left_offset, right_offset, bottom_offset;
  float gridRowHeight;
  float gridColWidth;

  void updateGrid();

  sf::RenderWindow& window;
};

#endif
