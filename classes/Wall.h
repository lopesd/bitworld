/*  Wall.h
 *  Can't move onto squares where there a walls. Walls don't move, and can't be corrupted
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef WALL_H
#define WALL_H

#include "CellGroup.h"

class Wall : public CellGroup {
  
 public:
 /** CONSTRUCTOR **/
  Wall( std::vector<Cell> cells );
 
	/** UTILITY **/
	
	//Override so that the wall doesn't get corrupted
  void dropResistance ( int n ) {} 

	/**ACCESSORS **/
	
	//used for checking if Bit is a Wall and Debugging
  std::string type () {return std::string("Wall");}

 private:
 	/** UTILITY **/
 
 //Checks all the "pieces" of the wall to merge them together into a more cohesive looking picture
  void setCellContexts ();
  
};

#endif
