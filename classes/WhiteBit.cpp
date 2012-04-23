#include "WhiteBit.h"
#include <stdlib.h>

using namespace std;

WhiteBit::WhiteBit (vector<Cell> c) : CellGroup (c) { 
  weight = 4;
  maxResistance = resistance = 5;
  for (int i = 0; i < cells.size(); ++i) {
    cells[i].setImage( "white_bit.png" );
  }
  movementDistance = 3;
}

//adds a bit that was in range of pulse to every White Bit's list of flagged units
void WhiteBit::addFlagged( CellGroup* newFlagged ){ 
  flaggedBits.push_back(newFlagged);
}

//chooses the closest of the flaggedBits to move towards/kill
void WhiteBit::findClosest(set<CellGroup*>& flaggedBits){
	Location temp;
	Location white = getLocations();
	int distance = -1; //intitalize to -1 for checking purposes
	for(int i = 0; i < flaggedBits.size(); i++){
		temp = flaggedBits[i]->getLocations();
		if(abs(temp.x - white.x + temp.y - white.y) > distance){ //if closer than the closest bit found so far, save that bit
			distance = abs(temp.x - white.x + temp.y - white.y);
			chosen = flaggedBits[i];
		}
	}

}
//attempts to take the shortest route to the closest flagged bit
Direction WhiteBit::findMove(){
	 srand ( time(NULL) ); //seeding rand for choosing between moving x or y when the distances from the flagged bit are the same
	 int odd = movementDistance % 2;
	 Direction newMove;
	 Location distanceAway;
	 int tempX = 0 = tempY = 0;
	 distanceAway.x = chosen->getMovement(0).x + chosen->getLocations().x - getLocations().x;
	 distanceAway.y = chosen->getMovement(0).y + chosen ->getLocations().y - getLocations().y;
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
		for(int j = 0; j < controlGroup->level->units.size(); j++){
		  if(controlGroup->level->units[i]->getMovement(0)+controlGroup->level->
		    return newMove;
		}
	 }

}

void WhiteBit::upCycle () {
  set<CellGroup*> flaggedBits = controlGroup->level->getFlaggedUnits();

  if(!(flaggedBits.empty()))
  {
	 findClosest(flaggedBits);
	 movementQueue.push_front(findMove());

  }
	CellGroup::upCycle();
}

