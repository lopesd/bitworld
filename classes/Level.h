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

class ControlGroup;
class CellGroup;

class Level {

 public:
  /** CONSTRUCTORS **/
  Level (sf::RenderWindow &window, std::vector<ControlGroup*>, std::vector<CellGroup*> units,
         int width=13, int height=9, int cpp=3);
  Level (const Level& L);

  /** UTITLITY FUNCTIONS **/
  /* USER INPUT */
  void prepareInput (int x, int y, int isRightClick);
  void handleInput  (Location  loc);
  void handleInput  (Direction dir);
  void handleInput  (sf::Key::Code);

  /* DRAWING */
  void draw             (int cycleOffset);
  void drawGrid         ();
  void drawUnits        ();
  void drawArrows       ();
  void drawCycle        ( int offset );
  void drawBackground   ();
  void highlightSelect  ();

  /* RUNNING */
  void controlGroupDone ();
  void runPeriod    ();
  void runCycle     ();
  int  willMove    (Location myLoc);
  std::vector<CellGroup*> findNeighbors (CellGroup*);

  /* GET--SET */
  int getTopOffset();
  int getBottomOffset();
  int getLeftOffset();
  int getRightOffset();
  int getCyclesPerPeriod();

 private:
  /** UNITS, CONTROLGROUPS, GRIDS **/
  ControlGroup*                     activeGroup;
  int                               activeGroupIndex;
  std::vector<ControlGroup*>        controlGroups;
  std::vector<CellGroup*>           units;
  std::map<Location, CellGroup*>    doubleBufferGrid [2];  //Double buffered grid
  std::map<Location, CellGroup*>*   grid;                 //Pointer to reference current grid
  int future;                                  //Indicates which grid contains info on future positions
  void resetGrid (); //Reads in data from the units vector and places it in the grid

  /** GRID INFO **/
  int width, height;   //Size of grid in terms of cells
  int cyclesPerPeriod;

  int top_offset, left_offset, right_offset, bottom_offset; //Offset used to draw the grid on the window
  float gridRowHeight;
  float gridColWidth;

  /** SFML OBJECTS **/
  sf::RenderWindow& window;
  sf::Sprite backgroundSprite;
  sf::Sprite highlightSprite;
  sf::Sprite arrowSprite;
  sf::Sprite stopSprite;
};

#endif
