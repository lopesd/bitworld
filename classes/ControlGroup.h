/** ControlGroup.h
 *  A ControlGroup represents a "player" in the game. It is the base class for the AI and User specific groups.
 *  It is responsible for keeping track of a selected unit and directing given input to it.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

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
  /** CONSTRUCTORS **/
  ControlGroup (std::vector<CellGroup*>);

  /** UTILITY FUNCTIONS **/
  virtual void startTurn   () = 0;
  virtual void handleInput (Direction dir) = 0;
  virtual void handleInput (CellGroup* unit) = 0;
  virtual void handleInput (sf::Key::Code) = 0;

  virtual void take    ( CellGroup* unitToTake    ); //Takes a unit into its possession
  virtual void forfeit ( CellGroup* unitToForfeit ); //Forfeits control of a unit

  /** ACCESSORS **/
  CellGroup* getSelectedUnit();
	int getUnitsSize();
	int getPlayer();
  /** MUTATORS **/
  void setLevel (Level* level);
  void clearSelection ();

  /** PUBLIC DATA MEMBERS **/
  Level* level;

 protected:
  std::vector<CellGroup*> units;
  CellGroup* selectedUnit;
	int player; //defines 0 as computer control group, 1 for a player

};

#endif
