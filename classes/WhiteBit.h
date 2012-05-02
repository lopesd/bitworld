/*  WhiteBit.h
 *  System protector that chases down flagged bits. Moves multiple spaces at once and
 *  can phase through walls. Doesn't move unless there is a flagged bit.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef WHITE_BIT_H
#define WHITE_BIT_H

#include "CellGroup.h"
#include <map>

#include <set>

class WhiteBit : public CellGroup {

 public:
 	/** CONSTRUCTOR **/
  WhiteBit (std::vector<Cell> cells);
  
  /** UTILITY FUNCTIONS **/
  
  //finds the move closest to the closest flagged bit that doesnt have another bit there
  Direction findMove ( std::map<Location, CellGroup*> grid, std::set<CellGroup*> flaggedUnits );
  
  //sets chosen to be the closest bit
  void findClosest ( std::set<CellGroup*> flaggedBits, std::map<Location, CellGroup*> grid );
  
  //Override the drop resistance function so the white bit cannot be corrupted 
  void dropResistance ( int n ) {}; 
  
  /** MUTATORS **/
  
  //sets the movement speed for the whiteBit
  void setSpeed ( int );
  
  /**ACCESSORS **/
  
  //used for checking if Bit is a whiteBit and Debugging
  std::string type () {return std::string("WhiteBit");}
  
 private:
  // Return to original position
  Direction phaseBack ();
  // Original position
  Location homeCell;
  
  CellGroup* chosen; //closest flagged bit
  Location chosenLoc; //future location of chosen bit
  int speed; //how far it can move in one cycle

};

#endif
