/** Byte.h
 *  The byte is a simple, lumbering, 4 cell big unit that has no further specialization.
 *  Make sure the cells passed to it are correctly placed (in a square formation).
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef BYTE_H
#define BYTE_H

#include "CellGroup.h"

class Byte : public CellGroup {

 public:
  /** CONSTRUCTORS **/
  // The cells passed should be correctly placed
  Byte ( std::vector<Cell> cells );

  /** ACCESSORS **/
  // Indicates what kind of unit it is
  std::string type () {return std::string("Byte");}

 private:
  // Set the images for each cell based on what cells are around it
  void setCellContexts ();

};

#endif
