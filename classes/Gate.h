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
  virtual void draw ( sf::RenderWindow &screen );
  virtual void setGridData ( int, int, int, int );
  // GAME LOGIC
  virtual Event highCycle ();

  /** MUTATORS **/
  void setWeight ( int );
  void setDest   ( std::string );

  /** ACCESSORS **/
  std::string destination ();

  /** PUBLIC DATA MEMBERS **/
  Level* level;

 private:
  std::vector<Cell> cells;
  std::string dest; // the destination level

  int weight; // how long it takes to open
  int openCounter; // counts down till the gate is open

};

#endif
