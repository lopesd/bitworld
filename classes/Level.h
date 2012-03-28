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
  Level (sf::RenderWindow &window, std::vector<ControlGroup*>, std::vector<CellGroup*> units,
         int width=13, int height=9, int cpp=3);
  Level (sf::RenderWindow &window);
  Level (const Level&);

  void init (std::vector<ControlGroup*>, std::vector<CellGroup*> units, int width=13, int height=9, int cpp=3);

  void display     ();
  void prepareInput(int x, int y, int isRightClick);
  void handleInput (Location  loc);
  void handleInput (Direction dir);
  void handleInput (sf::Key::Code);
  void run         ();
  std::vector<CellGroup*> findNeighbors (CellGroup*);
  void drawGrid    ();
  void drawUnits   ();
  void highlightSelect();

  void drawArrows  ();
  void drawBackground();

  void drawCycle( int offset );

 private:
  ControlGroup*                activeGroup;
  std::vector<ControlGroup*>        controlGroups;
  std::vector<CellGroup*>           units;
  std::map<Location, CellGroup*>    grid;
  int width, height;
  int cyclesPerPeriod;

  int top_offset, left_offset, right_offset, bottom_offset;
  float gridRowHeight;
  float gridColWidth;

  void updateGrid();

  sf::RenderWindow& window;
  sf::Sprite backgroundSprite;
};

#endif
