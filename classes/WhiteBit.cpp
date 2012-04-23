#include "WhiteBit.h"
#include <stdlib.h>

#include <set>

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  maxResistance = resistance = 5;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" ); 
    cells[i].setMovementAnimation( WHITEBIT );
  }
  movementDistance = 3;
  chosen = 0;
}

/*
void WhiteBit::getMovement( int n ) {
  Direction dir;

  if( controlGroup->level->getFlaggedUnits.empty() ) {
    dir.x = 0; dix.y = 0;
  } else {
    findClosest( controlGroup->level->getFlaggedUnits() );
    dir = findMove();
  }

  return dir;
}
*/

//chooses the closest of the flaggedBits to move towards/kill
void WhiteBit::findClosest( set<CellGroup*> flaggedBits ){
  
  if( flaggedBits.empty() ) {
    chosen = 0;
    return;
  }
  
  Location temp;
  Location white = getLocations()[0];
  int distance = -1; //intitalize to -1 for checking purposes
  for( set<CellGroup*>::iterator it = flaggedBits.begin(); it != flaggedBits.end(); ++it ){
    temp = (*it)->getLocations()[0];
    if(abs(temp.x - white.x + temp.y - white.y) > distance){ //if closer than the closest bit found so far, save that bit
      distance = abs(temp.x - white.x + temp.y - white.y);
      chosen = *it;
    }
  }

}

//attempts to take the shortest route to the closest flagged bit
Direction WhiteBit::findMove ( map<Location, CellGroup*> grid ) {

  findClosest( controlGroup->level->getFlaggedUnits() );
  if( !chosen ) {
    Direction dir = {0,0};
    return dir;
  }

  srand ( time(NULL) ); //seeding rand for choosing between moving x or y when the distances from the flagged bit are the same
  int odd = movementDistance % 2;
  Direction newMove;
  Location distanceAway;
  int tempX  = 0;
  int tempY = 0;
  distanceAway.x = chosen->getMovement(0).x + chosen->getLocations()[0].x - getLocations()[0].x;
  distanceAway.y = chosen->getMovement(0).y + chosen ->getLocations()[0].y - getLocations()[0].y;
  for(int i = 0; i < movementDistance && abs(distanceAway.x)-tempX > 0 && abs(distanceAway.y)-tempY > 0; i++){
    if(abs(distanceAway.y) - tempY < abs(distanceAway.x) - tempX)
      tempX++;
    else if(abs(distanceAway.y) - tempY > abs(distanceAway.x) - tempX)
      tempY++;
    else{
      if(rand() % 2 + 1)
	tempY++;
      else
	tempX++;
    }

  }
  if(distanceAway.y < 0)
    newMove.y = -tempY;
  else
    newMove.y = tempY;
  if(distanceAway.x < 0)
    newMove.x = -tempX;
  else
    newMove.x = tempX;
  //for(int j = 0; j < controlGroup->level->units.size(); j++){
  //if(controlGroup->level->units[i]->getMovement(0)+controlGroup->level->
  return newMove;
}

void WhiteBit::upCycle () {
  

  CellGroup::upCycle();
}
