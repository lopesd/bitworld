/** Bit.h
 *  The simplest of Bits, this bit does nothing but follow given orders.
 *  It is hardly specialized any more than CellGroup.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#ifndef BIT_H
#define BIT_H

#include "CellGroup.h"

class Bit : public CellGroup {

 public:
  /** CONSTRUCTORS **/
  Bit (std::vector<Cell> cells);

  /** ACCESSORS **/
  // Useful for debugging.
  std::string type () {return std::string("Bit");}

 private:

};

#endif
