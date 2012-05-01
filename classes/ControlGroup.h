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

class Level;     //Forward declarations
class CellGroup; 

class ControlGroup {

 public:
  /** CONSTRUCTORS **/
  // Hand it the CellGroups that will be under its control
  ControlGroup (std::vector<CellGroup*>);

  /** UTILITY FUNCTIONS **/
  // PURE VIRTUAL FUNCTIONS //
  // Start the Control Group's turn.
  virtual void startTurn   () = 0;

  // Input handlers
  virtual void handleInput (Direction dir) = 0;
  virtual void handleInput (CellGroup* unit) = 0;
  virtual void handleInput (sf::Key::Code) = 0;
  
  //Takes a unit into its possession
  virtual void take    ( CellGroup* unitToTake    );
 //Forfeits control of a unit 
  virtual void forfeit ( CellGroup* unitToForfeit );
  // Deselect a unit, if one is selected
  void clearSelection ();

  /** ACCESSORS **/
  // Returns the selected unit
  CellGroup* getSelectedUnit();

  // Returns a vector of units that are under its control
  std::vector<CellGroup*> getUnits();
  // Returns size of units vector
  int getUnitsSize();

  // Returns 1 if the Control Group is a user controlled
  int getPlayer();

  /** MUTATORS **/
  // Set level overlord
  void setLevel (Level* level);

  /** PUBLIC DATA MEMBERS **/
  Level* level;

 protected:
  // The units under control of the Control Group
  std::vector<CellGroup*> units;

  // Pointer to the selected unit -- null if no unit is selected
  CellGroup* selectedUnit;

  // Defines 0 as computer control group, 1 for a player
  int player; 
	
};

#endif
