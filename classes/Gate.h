/** Gate.h
 *  A gate does not derive from CellGroup and is not a "unit" per se.
 *  It is responsible for drawing its cells, and is the base class for other specialized gates.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#ifndef GATE_H
#define GATE_H

#include <SFML/Graphics.hpp> 

#include <vector>
#include <string>
#include <set>

#include "structures.h"
#include "Cell.h"
#include "Level.h"
#include "Event.h"

class Level;
class Event;

class Gate {

 public:
  /** CONSTRUCTORS **/
  Gate( std::vector<Cell> cells );

  /** UTILITY METHODS **/
  // DRAWING AND ANIMATION
  // Determine the image for each cell
  virtual void setCellContexts ();
  virtual void setCellImages ();
  virtual void draw ( sf::RenderWindow &screen );
  // GAME LOGIC
  virtual void highCycle ();

  /** MUTATORS **/
  void setLocked        ( int );
  void setGridData      ( int, int, int, int );
  void setWeight        ( int );
  void setDest          ( std::string );
  void resetOpenCounter ();

  /** ACCESSORS **/
  int getLocked ();
  std::string destination ();
  std::set<CellGroup*> getUnitsToTransfer ();
  std::vector<Location> getLocations ();

  /** PUBLIC DATA MEMBERS **/
  Level* level;
  int    tag;
  int    destinationTag;

 private:
  std::vector<Cell> cells;
  std::set<CellGroup*> unitsToTransfer;
  std::string dest; // the destination level

  int weight; // how long it takes to open
  int openCounter; // counts down till the gate is open
  int locked; // determines if the gate opens or not
  
};

#endif
