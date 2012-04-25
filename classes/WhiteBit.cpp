#include "WhiteBit.h"
#include <stdlib.h>

#include <set>

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  maxResistance = resistance = 5;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" ); 
    cells[i].setMovementAnimation( PHASE );
  }
  speed = 3;
  chosen = 0;
}

//chooses the closest of the flaggedBits to move towards/kill
void WhiteBit::findClosest( set<CellGroup*> flaggedBits, map<Location, CellGroup*> grid ){
  
  if( flaggedBits.empty() ) {
    chosen = 0;
    return;
  }

  cout << endl << "FINDING CLOSEST FLAGGED BIT " << endl;
  Location temp;
  Location white = getLocations()[0];
  int distance = 100; //intitalize to a large value for checking purposes
  for( set<CellGroup*>::iterator it = flaggedBits.begin(); it != flaggedBits.end(); ++it ) { //search the future grid
    for( map<Location, CellGroup*>::iterator jt = grid.begin(); jt != grid.end(); ++jt ) {   //for the location of the chosen bit
      if( jt->second == *it ) {
	
	temp = jt->first;
	if( abs(temp.x - white.x + temp.y - white.y) < distance) { //if closer than the closest bit found so far, save that bit
	  distance = abs(temp.x - white.x + temp.y - white.y);
	  chosen = *it;
	  chosenLoc = temp;
	}

      }
	
    }
  }

  cout << "Chose to chase " << chosen->type() << ", at location " << temp << endl << endl;

}

//attempts to take the shortest route to the closest flagged bit
Direction WhiteBit::findMove ( map<Location, CellGroup*> grid, set<CellGroup*> flaggedUnits ) {

  findClosest( flaggedUnits, grid );
  if( !chosen ) {
    Direction dir = {0,0};
    return dir;
  }

  vector<Direction> possibles;
  for( int i = -speed; i <= speed; ++i ) {
    for( int j = 0; j <= speed-abs(i); ++j ) {

      Direction dir = {i,j};
      Location floc = getLocations()[0] + dir;

      //bounds checking
      if( floc.x < 0 || floc.y < 0 || floc.x >= controlGroup->level->getWidth() || floc.y >= controlGroup->level->getHeight() ) {
	cout << floc << ", in direction " << dir << " is out of range." << endl;
      } else {
	
	if( grid.find(floc) != grid.end() ) { //if someone is already there
	  if( grid[floc] == chosen ) {
	    return dir; //WE HAVE FOUND THE CHOSEN ONE
	  } 
	  else if( grid[floc] == this ) {  //in case of (0,0), the one there is myself.
	    cout << "Adding " << floc << " to possibles, because I am there " << endl;
	    possibles.push_back( dir );
	  }
	} else { //there is no one in the way, add to possibles
	  possibles.push_back( dir );
	}
      }
      
      if( j ) {

	Direction dir2 = {i,-j};
	floc = getLocations()[0] + dir2;

	if( floc.x < 0 || floc.y < 0 || floc.x >= controlGroup->level->getWidth() || floc.y >= controlGroup->level->getHeight() ) {
	  cout << floc << ", in direction " << dir2 << " is out of range." << endl;
	} else {
	  
	  cout << "Checking location " << floc << " in direction " << dir2 << endl;
	  if( grid.find(floc) != grid.end() ) {
	    if( grid[floc] == chosen ) {
	      return dir2; //WE HAVE FOUND THE CHOSEN ONE
	    } 
	  } else { //there is no one in the way, add to possibles
	    possibles.push_back( dir2 );
	  }
	}	
      }

    }
  }
  
  Direction best = {0,0};
  Direction smallestDist = {controlGroup->level->getWidth(),controlGroup->level->getHeight()};
  vector<Location> myLocs = getLocations();
  for( int j = 0; j < myLocs.size(); ++j )
    for( int i = 0; i < possibles.size(); ++i ) {

      Direction distance;
      distance.x = abs(chosenLoc.x - (myLocs[j].x+possibles[i].x));
      distance.y = abs(chosenLoc.y - (myLocs[j].y+possibles[i].y));

      if( distance < smallestDist ) {
	smallestDist = distance;
	best = possibles[i];
      }

    }
  
  return best;

}

void WhiteBit::setSpeed ( int s ) {
  speed = s;
}
