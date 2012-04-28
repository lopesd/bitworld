/** Gate.cpp
 *  A gate does not derive from CellGroup and is not a "unit" per se.
 *  It is responsible for drawing its cells, and is the base class for other specialized gates.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Gate.h"

using namespace std;

Gate::Gate ( vector<Cell> c ) {
  cells = c;
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].setImage( "new_gate_open.png" );
  }
  weight = 3;
  openCounter = weight;
}

void Gate::draw ( sf::RenderWindow &screen ) {
  for( int i = 0; i < cells.size(); ++i ) {
    cells[i].draw( screen );
  }
}

void Gate::setGridData ( int w, int h, int t, int l ) {
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setGridData( w, h, t, l );
  }
}

void Gate::highCycle () {
  int foundUnit = 0;
  for( int i = 0; i < cells.size(); ++i ) { 
    CellGroup* unitOnMe = level->unitAtLocation( cells[i].getGridLocation() );
    if( unitOnMe ) {
      foundUnit = 1;
      --openCounter;
    }
  }
  if( !foundUnit ) openCounter = weight;

  if( openCounter <= 0 ) { //Gate opens
    level->openGate( this );
  }
}

void Gate::setWeight ( int w ) {
  weight = w;
  openCounter = weight;
}

void Gate::setDest ( std::string s ) {
  dest = s;
}

void Gate::resetOpenCounter () {
  openCounter = weight;
}

string Gate::destination () {
  return dest;
}
