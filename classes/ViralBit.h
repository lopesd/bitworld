/*  ViralBit.cpp
 *  The starting user bit. It can corrupt other bits by staying adjacent to them
 *  for a certain number of cycles depending on the unit's resistance.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */
#ifndef VIRAL_BIT_H
#define VIRAL_BIT_H

#include "CellGroup.h"

class ViralBit : public CellGroup {
  
 public:
 
 	/** CONSTRUCTOR **/
  ViralBit  (std::vector<Cell> cells);

	/** UTILITY **/
	
	//corrupts in its downCycle
	void downCycle (); 
	
	/** Accessors **/
	
	//for Debugging and checking of type of bit
  std::string type () {return std::string("ViralBit");} 

 private:

};

#endif
