#include "Level.h"

#include <iostream> //Remove later
#include <unistd.h> //For terminal version

Level::Level (vector<ControlGroup*> c, vector<CellGroup*> u, int w, int h, int cpp) {
  controlGroups = c;
  activeGroup = c.at(0); // set active group to be the first in the list
  units = u;
  width = w;
  height = h;
  cyclesPerPeriod = cpp;

  updateGrid();
}

void Level::updateGrid () { // Clears and remakes the entire grid
  // Clear grid first
  grid.clear();

  vector<Location> locs;

  for (int i = 0; i < units.size(); ++i) {
    locs = units.at(i)->getLocations(); // Get all the locations of a cellGroup (could be more than 1)
    for (int j = 0; j < locs.size(); ++j) { // Set all a unit's positions to pointers to itself
      cout << "Inserting pointer at location: " << locs.at(j).x << ", " << locs.at(j).y << endl;
      grid.insert( pair<Location, CellGroup*>( locs.at(j), units.at(i) ) );  
    }
  }

  int count = 0;
  for ( map<Location, CellGroup*>::iterator i = grid.begin(); i != grid.end(); ++i, count++) {
    cout << "Elements in the grid, " << count << ": " << i->first.x << " " << i->first.y << endl;
  }

}
 
void Level::display () {
  char img = '-';
  Location loc;
  
  int unitsFound = 0;
  for (int i = 0; i < width; ++i) {
    cout << endl;
    for (int j = 0; j < height; ++j) {
      img = '-';
      loc.x = j; loc.y = i;
      
      if ( grid.find( loc ) != grid.end() ) {
	unitsFound++;
	img = grid[loc]->getImage();
      } 
      
      cout << img << " ";
    }
  }

  Location test = {1, 2};
  if (grid.find(test) != grid.end() ) cout << "Found something at " << test << endl;
  test.x = 2; test.y = 1;
  if (grid.find(test) != grid.end() ) cout << "Found something at " << test << endl;
  
  cout << "Units found: " << unitsFound << endl 
       << "Size of grid: " << grid.size() << endl;
  usleep(400000);
}

void Level::handleInput (Location loc) {

  cout << "Location: " << loc.x << ", " << loc.y << endl;
  map<Location, CellGroup*>::iterator clickedUnit = grid.find( loc );
  
  if (clickedUnit != grid.end() ) {
    cout << "There is a unit at that location." << endl;
    clickedUnit->second->controlGroup->handleInput ( clickedUnit->second );
  }
  else {
    cout << "NO UNIT at that location." << endl;
    CellGroup* nullPointer = 0;
    activeGroup->handleInput ( nullPointer );
  }

}

void Level::handleInput (Direction dir) {
  activeGroup->handleInput (dir);
}

void Level::run () {
  //commit all movements, doing collision detection and stuff
  //cycle a certain number of times
  
  for (int i = 0; i < cyclesPerPeriod; ++i) {
    for (int j = 0; j < units.size(); ++j) {
      units.at(j)->upCycle ();
    }

    updateGrid();
    display();
    usleep(100000);
  }

}
