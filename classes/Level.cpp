/** Level.cpp
 *  The level object contains all the information about the grid for a certain level.
 *  It receives and interprets input from the user and controls ControlGroups.
 *  It is also responsible for drawing the grid portion of the screen.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "InfoBox.h"

#include <SFML/Graphics.hpp>

#include "ControlGroup.h"
#include "CellGroup.h"
#include "Event.h"
#include "Gate.h"
#include "WhiteBit.h"

#include <iostream> //Remove later
#include <unistd.h> //For usleep function
#include <algorithm> //For the find in vector function
#include <cstring>

using namespace std;

#define ARROW_HEIGHT 50
#define ARROW_LENGTH 50

extern int FPS;

/** CONSTRUCTORS **/
Level::Level (sf::RenderWindow &newWindow, vector<ControlGroup*> c, vector<CellGroup*> u, vector<Gate*> g,
              int w, int h, int cpp)
: window(newWindow)
{
  controlGroups = c;
  for( int i = 0; i < controlGroups.size(); ++i ) // Tell controlGroups to recognize me as their level overlord
    controlGroups[i]->setLevel( this );
  units = u;
  gates = g;
  for( int i = 0; i < gates.size(); ++i )
    gates[i]->level = this;
  width = w;
  height = h;
  cyclesPerPeriod = cpp;
  cyclesToRun = 0;
  isDone = 0;

  grid = doubleBufferGrid;
  resetGrid();
  future = 1;

  // Calculate location of grid to be drawn
  FloatPair center;
  center.x = (float)(window.GetWidth())/2;
  center.y = (float)(window.GetHeight())/3;
  gridRowHeight = 50;
  gridColWidth  = 50;

  left_offset = center.x - (float)(width)/2*gridColWidth;
  top_offset  = center.y - (float)(height)/2*gridRowHeight;
  bottom_offset = window.GetHeight() - top_offset - gridRowHeight*height;
  right_offset = left_offset;
  deafFrames = 0;
  cycleOffset = 20;

  for (int i = 0; i < units.size(); ++i)
    units[i]->setGridData( gridColWidth, gridRowHeight, top_offset, left_offset );
  for (int i = 0; i < gates.size(); ++i)
    gates[i]->setGridData( gridColWidth, gridRowHeight, top_offset, left_offset );

  backgroundSprite.SetImage( ImageCache::GetImage("Dark.jpg") );
  backgroundSprite.Resize( window.GetWidth(), window.GetHeight() );
  highlightSprite.SetImage ( ImageCache::GetImage("blue_transparent.png") );
  highlightSprite.SetCenter( highlightSprite.GetSize() / 2.f );
  highlightSprite.Resize( gridColWidth, gridRowHeight );
  arrowSprite.SetImage( ImageCache::GetImage( "arrow3.png" ) );
  arrowSprite.SetCenter( arrowSprite.GetSize() / 2.f );
  arrowSprite.Resize( gridColWidth, gridRowHeight );
  stopSprite.SetImage( ImageCache::GetImage( "stop_diamond2.png" ) );
  stopSprite.SetCenter( stopSprite.GetSize() / 2.f );
  stopSprite.Resize( gridColWidth / 2, gridRowHeight / 2 );

  activeGroupIndex = 0;
  activeGroup = controlGroups[0]; // set active group to be the first in the list
  activeGroup->startTurn();
}

Level::Level (const Level& L) : window(L.window) {
  activeGroupIndex = L.activeGroupIndex;
  controlGroups = L.controlGroups;
  units = L.units;
  gates = L.gates;
  doubleBufferGrid[1] = L.doubleBufferGrid[1];
  doubleBufferGrid[0] = L.doubleBufferGrid[0];
  grid = doubleBufferGrid;
  future = L.future;
  width = L.width;
  height = L.height;
  cyclesPerPeriod = L.cyclesPerPeriod;
  cyclesToRun = L.cyclesToRun;
  isDone = 0;

  for( int i = 0; i < controlGroups.size(); ++i) // Tell controlGroups to recognize me as their level overlord
    controlGroups[i]->setLevel( this );
  for( int i = 0; i < gates.size(); ++i )
    gates[i]->level = this;

  top_offset = L.top_offset;
  left_offset = L.left_offset;
  right_offset = L.right_offset;
  bottom_offset = L.bottom_offset;
  gridRowHeight = L.gridRowHeight;
  gridColWidth = L.gridColWidth;
  deafFrames = L.deafFrames;
  cycleOffset = L.cycleOffset;

  /** SFML OBJECTS **/
  backgroundSprite = L.backgroundSprite;
  highlightSprite = L.highlightSprite;
  arrowSprite = L.arrowSprite;
  stopSprite = L.stopSprite;

  activeGroup = controlGroups[activeGroupIndex];
}

void Level::destroy () {
  for( int i = 0; i < units.size(); ++i ) {
    delete units[i];
  }
  for( int i = 0; i < controlGroups.size(); ++i ) {
    delete controlGroups[i];
  }
  for( int i = 0; i < gates.size(); ++i ) {
    delete gates[i];
  }
}

void Level::resetGrid () { // Clears and remakes the entire grid
  grid[0].clear();

  vector<Location> locs;

  for (int i = 0; i < units.size(); ++i) {
    locs = units.at(i)->getLocations(); // Get all the locations of a cellGroup (could be more than 1)
    for (int j = 0; j < locs.size(); ++j) { // Set all a unit's positions to pointers to itself
      grid[0].insert( pair<Location, CellGroup*>( locs[j], units[i] ) );
    }
  }

}

/** UTILITY FUNCTIONS **/
// USER INPUT
void Level::prepareInput(int x, int y, int isRightClick) {
  Location Lorder;

  if(y > top_offset && y < window.GetHeight() - bottom_offset &&
			   x > left_offset && x < window.GetWidth() - right_offset) {
    Lorder.x = (x - left_offset) / gridColWidth;
    Lorder.y = (y - top_offset)  / gridRowHeight;
  }

  if (!isRightClick)
    handleInput(Lorder);
  else {
    CellGroup* unit = activeGroup->getSelectedUnit();
    if (unit) {
      Direction Dorder;
      FloatPair middle = unit->getPathHead();

      int xdir = middle.x - Lorder.x;
      int ydir = middle.y - Lorder.y;

      if ( ((xdir == 1 || xdir == -1) && ydir == 0) ||
	   ((ydir == 1 || ydir == -1) && xdir == 0) ||
	    (xdir == 0 && ydir ==  0) ) {
	Dorder.x = Lorder.x - middle.x;
	Dorder.y = Lorder.y - middle.y;
	handleInput(Dorder);
      }
    }
  }
}

void Level::handleInput (Location loc) {

  if( deafFrames ) return;

  map<Location, CellGroup*>::iterator clickedUnit = grid[0].find( loc );

  if (clickedUnit != grid[0].end() ) {
    activeGroup->handleInput ( clickedUnit->second );
  }
  else {
    CellGroup* nullPointer = 0;
    activeGroup->handleInput ( nullPointer );
  }
}

void Level::handleInput (Direction dir) {
  if( deafFrames ) return;
  activeGroup->handleInput (dir);
}

void Level::handleInput (sf::Key::Code keyPressed) {
  if( deafFrames ) return;
  activeGroup->handleInput (keyPressed);
}


/* RUNNING */
// Called by a CG when it has finished its turn
void Level::controlGroupDone () {
 //Loop through the controlGroups. Run period when all have had their turn.
  if( ++activeGroupIndex >= controlGroups.size() ) {
    activeGroupIndex = 0;
    runPeriod();
  }

  activeGroup = controlGroups[activeGroupIndex];
  activeGroup->startTurn();
}

void Level::runPeriod () {
  cyclesToRun = cyclesPerPeriod;
}

void Level::runCycle () {

  cycleOffset = 19; //Set offset for the cycle animation
  requestDeafFrames( 20 );

  Location fLoc;
  Direction tempDir;
  vector<Location> locs;

  // COLLISION DETECTION
  for( int i = 0; i < units.size(); ++i) { //For every unit
    if( strcmp( units[i]->type().c_str(), "WhiteBit" ) == 0 ) continue; //don't check white bits
    
    locs =    units[i]->getLocations();
    tempDir = units[i]->getMovement(0);     //Get my next desired movement

    for( int j = 0; j < locs.size(); ++j) {  //For every piece of that unit
      // IF THE CELL WISHES TO STAY PUT
      if( tempDir.isZero() ) { //The unit must not be a whitebit
	grid[future][locs[j]] = units[i]; //It gets to stay put.
	continue;
      }

      fLoc = locs[j]+tempDir; //My desired future location

      if( willMove(locs[j]) ) {

	if( grid[future].find(fLoc) == grid[future].end() ) {
	  grid[future][fLoc] = units[i]; //I am allowed to move.
	} else { //ABSORPTION	  
	  handleMerge( units[i], grid[future][fLoc], fLoc );
	}
      }
      
      else { //COLLISION IS HEAD-ON -- apply directly to the forehead
	for( int k = 0; k < j; ++k ) //Clear all of my previous positions that have been placed
	  grid[future].erase( grid[future].find( locs[k]+tempDir ) ); //Remove that pointer
	for( int k = 0; k < locs.size(); ++k ) //And for every one of my present locations
	  grid[future][locs[k]] = units[i];    //Make those my future locations too. Effectively, keep me in place.
	units[i]->setFreeToMove(0);
	break; //Don't check my other pieces.
      }
    }
  }

  //Move white bits
  for( int i = 0; i < units.size(); ++i ) 
    if( strcmp( units[i]->type().c_str(), "WhiteBit" ) == 0 ) {
      if( !flaggedUnits.empty() ) {
	Direction dir = ((WhiteBit*)units[i])->findMove( grid[future], flaggedUnits ); //The white bit finds its desired move
      
	units[i]->issueMovementOrder( dir );
	
	vector<Location> myLocs = units[i]->getLocations();
	for( int j = 0; j < myLocs.size(); ++j ) {
	  Location floc = myLocs[j] + dir;
	  if( grid[future].find( floc ) != grid[future].end() ) {
	    if( grid[future][floc] == units[i] ) cout << "I'M EATING ME GAAAAH" << endl;
	    handleMerge( units[i], grid[future][floc], floc );
	  }
	}
      }
    }
  
  //Move all units (only the ones that are free to move will move)
  for( int i = 0; i < units.size(); ++i )
    units[i]->upCycle ();
  
  //DOUBLE BUFFERING -- Switch present grid with future grid
  grid[0].clear(); //Clear old grid
  grid = grid + future;
  future = -future;

  // HIGH CYCLES
  vector<Event> tempEvs;
  Event ev;
  for( int i = 0; i < gates.size(); ++i ) {
    ev = gates[i]->highCycle();
    if( ev.type == OPEN ) {
      isDone = 1;
      Gate* temp = (Gate*)ev.sender;
      destination = temp->destination();
    }
  }

  // DOWN CYCLES (NEGATIVE EDGE OF CLOCK)
  // Detect bit death first
  for( int i = 0; i < units.size(); ++i ) {
    int dontDie = 0;
    vector<Location> locs = units[i]->getLocations();
    for( int j = 0; j < locs.size(); ++j ) { //for each cell of each unit,
      Location temp[4] = {  //check all neighboring locations
	{locs[j].x + 1, locs[j].y},
	{locs[j].x - 1, locs[j].y},
	{locs[j].x, locs[j].y + 1},
	{locs[j].x, locs[j].y - 1}
      };

      for( int k = 0; k < 4; ++k )
	if( grid[0].find(temp[k]) == grid[0].end() ) //if an adjacent square is empty
	  dontDie = 1;
    }

    if( !dontDie )
      unitsToDie.insert( units[i] );
  }
  
  for( int i = 0; i < units.size(); ++i ) {
    tempEvs = units[i]->downCycle();
    
    for( int j = 0; j < tempEvs.size(); ++j ) {
      if( tempEvs[j].type != EMPTY ) {
	handleEvent( tempEvs[j] );
	events.push_back( tempEvs[j] );
      }

    } //End events of unit loop
    
  } //End units loop

}

//Recursive checking if the unit currently at myLoc will move (eg. no head-on collision)
int Level::willMove ( Location myLoc ) {
  if( grid[0].find( myLoc ) == grid[0].end() ) cout << "could not find myLoc. OH SHIT SEGFAULT" << endl;
  
  CellGroup* unit = grid[0][myLoc]; //the unit that owns a cell at the given location

  //always return 1 if the unit is a white bit and if there are flaggedUnits it should be chasing.
  cout << "Cheking if " << unit->type() << " will move... ";
  if( strcmp( unit->type().c_str(), "WhiteBit" ) == 0 && !flaggedUnits.empty() ) {
    cout << "It is a moving whitebit!";
    return 1;
  }
  cout << endl;

  Direction myDir = unit->getMovement(0);
  Location fLoc = myLoc + myDir; //My desired future location
  if( (fLoc.x < 0) || (fLoc.y < 0) || (fLoc.x >= width) || (fLoc.y >= height) ) return 0; //Future position is off grid; do not move
  if( (myDir.isZero()) || (grid[0].find(fLoc) == grid[0].end()) ) return 1;   //There is no one there (or I am stopped); I go (there may still be an absorption)
  else { //If there is someone there...
    if( (fLoc + grid[0][fLoc]->getMovement(0) ) == myLoc ) return 0; //unit wants to move to where I am; head-on
    if( grid[0][fLoc]->getMovement(0).isZero() )           return 0; //unit wants to remain where it is; head-on
    return( willMove(fLoc) );                                        //I am free to move only if my future position clears up.
  }
}

void Level::handleEvent( Event ev ) {

  switch( ev.type ) {
    
  case PULSE : //loops through grid pulseRadius distance away and if a user bit is detected, add that cellGroup to the white bit's flagged vector
    for(int j = 0; j < ev.locations.size(); j++){
      if(grid[0].find(ev.locations[j]) != grid[0].end() ) { //If there is a unit at that location
	
	CellGroup* pulsedUnit = grid[0][ev.locations[j]];
	if( ((CellGroup*)ev.sender)->controlGroup != pulsedUnit->controlGroup ) {
	  flaggedUnits.insert( pulsedUnit );
	}
      }
    }
    break;
    
  case CORRUPT:
    for( int k = 0; k < ev.units.size(); ++k ) {
      ev.units[k]->controlGroup->forfeit( ev.units[k] );
      ((CellGroup*)ev.sender)->controlGroup->take( ev.units[k] );
    }
    break;
    
  }

}

// Handles a merge or absorption between two units who move to the same location
void Level::handleMerge ( CellGroup* unit1, CellGroup* unit2, Location loc ) {

  cout << "Merge Detected at " << loc << " between a " << unit1->type() << " and a " << unit2->type() << endl;

  // If they are allowed to merge into one big unit...
  if( 0 ) {
    // Merge them
  }

  else { // Someone will die and someone will live
    CellGroup* unitToDie;
    CellGroup* unitToLive;

    //Determine who must die
    if( unit1->getWeight() == unit2->getWeight() ) { //Equal weights
      if( strcmp(unit1->CGGroupName.c_str(), "user") == 0 ) { //If the unit belongs to user, give it precedence.
	unitToDie = unit2;
	unitToLive = unit1;
      }
      else {
	unitToDie = unit1;
	unitToLive = unit2;
      }
    }
    else if( unit1->getWeight() > unit2->getWeight() ) { //If I have precedence
      unitToDie  = unit2;  // KILL THE OTHER UNIT
      unitToLive = unit1;
    }
    else {
      unitToDie = unit1;  // KILL ME
      unitToLive = unit2;
    }

    //And do the appropriate flagging.
    unitsToDie.insert( unitToDie );
    if( flaggedUnits.find(unitToDie) != flaggedUnits.end() ) //if he was flagged, unflag him
      flaggedUnits.erase( flaggedUnits.find(unitToDie) );
    grid[future][loc] = unitToLive;
  }
}

// Removes the unit completely
void Level::killUnit ( CellGroup* unitToDie ) {
  vector<Location> deadLocs = unitToDie->getLocations();
  for( int d = 0; d < deadLocs.size(); ++d ) {
    if( grid[0].find(deadLocs[d])->second == unitToDie ) {
      grid[0].erase( grid[0].find(deadLocs[d]) );  //Erase all current positions, if they belong to me
    }
  }
  units.erase( find( units.begin(), units.end(), unitToDie ) ); //Remove him from the units vector
  delete unitToDie;                 //Finally, deallocate him
  if( activeGroup->getSelectedUnit() == unitToDie ) activeGroup->clearSelection();
  if( flaggedUnits.find( unitToDie ) != flaggedUnits.end() ) flaggedUnits.erase( flaggedUnits.find(unitToDie) );
}


/* DRAWING */
void Level::draw() {
  // deafFrames is how long the level will ignore user input. We decrease the count by 1 each frame.
  if( deafFrames ) --deafFrames;
  if( deafFrames == 0 ) {

    //Kill any units that must die
    if( !unitsToDie.empty() ) {
      for( set<CellGroup*>::iterator i = unitsToDie.begin(); i != unitsToDie.end(); ++i )
	killUnit( *i );
      unitsToDie.clear();
    }
    
    if( cyclesToRun != 0 && !isDone ) { // run cycles if we still need to
      --cyclesToRun;
      runCycle();
    }
    else { //if there are no deaf frames and no cycles to run, then gates must be reset
      for( int i = 0; i < gates.size(); ++i )
	gates[i]->resetOpenCounter();
    }

  }

  window.Clear();

  drawBackground();

  drawGrid();
  highlightSelect();
  drawGates();
  drawUnits();
  drawArrows();

  if     ( cycleOffset == 0 ) cycleOffset = 20;
  else if( cycleOffset != 20 ) --cycleOffset;
  drawCycle(20-cycleOffset);
}

void Level::drawGrid() {
  sf::Color gridColor(sf::Color(143, 114, 19, 0));
  sf::Color gridOutlineColor(sf::Color(100, 100, 240, 0));

  sf::Shape horLine;
  sf::Shape vertLine;
  int addLength;
  int addHeight;

  for(float scale = 0.08; scale >= 0; scale -= 0.01)
  {
    gridColor += sf::Color(0, 0, 0, 25);
    addLength = scale * (window.GetWidth() - left_offset - right_offset) - 10;

    horLine = sf::Shape::Line(left_offset - addLength, top_offset,
                            window.GetWidth() - right_offset + addLength, top_offset,
                            2, gridColor,
                            1, gridOutlineColor);

    window.Draw(horLine);

    for(int row = 0; row < height; row++)  //Draw horizontal lines
      {
      horLine.Move(0, gridRowHeight);
      window.Draw(horLine);
    }

    horLine.Move(0, -1 * gridRowHeight * height);

    addHeight = scale * (window.GetHeight() - top_offset - bottom_offset) - 10;
    vertLine = sf::Shape::Line(left_offset, top_offset - addHeight,
                               left_offset, window.GetHeight() - bottom_offset + addHeight,
                               2, gridColor,
                               1, gridOutlineColor);

    window.Draw(vertLine);
    for(int col = 0; col < width; col++)  //Draw vertical lines
      {
	vertLine.Move(gridColWidth, 0);
	window.Draw(vertLine);
      }
    vertLine.Move(0, -1 * gridColWidth * width);

  }
}

void Level::drawUnits() {
  for (int i = 0; i < units.size(); ++i)
    units[i]->draw( window );
}

void Level::drawGates() {
  for( int i = 0; i < gates.size(); ++i )
    gates[i]->draw( window );
}

void Level::drawArrows()
{
  vector<Location> groupLocations;
  CellGroup* unit;
  unit = activeGroup->getSelectedUnit();

  if(unit == 0)
    return;

  /*
  sf::Color darkBlue = sf::Color(0, 92, 9, 200);

  for(int count = 0; count < unit->numOfMovements(); count++)
  {
    window.Draw(sf::Shape::Rectangle(left_offset + ARROW_LENGTH * count,
                                     window.GetHeight() - bottom_offset,
                                     left_offset + ARROW_LENGTH * (count + 1),
                                     window.GetHeight() - bottom_offset + ARROW_HEIGHT,
                                     sf::Color(50, 50, 50, 100)));
    window.Draw(sf::Shape::Rectangle(left_offset + ARROW_LENGTH * (count + 0.3),
                                     window.GetHeight() - bottom_offset + ARROW_HEIGHT * 0.3,
                                     left_offset + ARROW_LENGTH * (count + 0.7),
                                     window.GetHeight() - bottom_offset + ARROW_HEIGHT * 0.7,
                                     darkBlue));
    int x1 = ARROW_LENGTH * -0.4;
    int x2 = ARROW_LENGTH * 0.4;
    int y1 = -ARROW_HEIGHT * 0.2;
    int y2 = -ARROW_HEIGHT * 0.5;

    sf::Shape Triangle;

    Triangle.SetPosition( (left_offset + ARROW_LENGTH * (count + 0.5)),
                         (window.GetHeight() - bottom_offset + ARROW_HEIGHT * 0.5));

    Triangle.AddPoint(x1, y1, darkBlue);
    Triangle.AddPoint(x2, y1, darkBlue);
    Triangle.AddPoint((x1 + x2) / 2, y2, darkBlue);

    switch((int)(unit->getMovement(count).x))
    {
      case 0:
        if(unit->getMovement(count).y == 1)
          Triangle.SetRotation(180);
        break;
      case -1:
        Triangle.SetRotation(90);
        break;
      case 1:
        Triangle.SetRotation(270);
        break;
    }

    window.Draw(Triangle);
  }
  */

  // DRAW ON GRID ARROWS
  // Create and position arrow sprite
  FloatPair arrowLocation = unit->getMiddle();
  arrowSprite.SetPosition( left_offset + gridColWidth*arrowLocation.x + gridColWidth/2,
                           top_offset + gridRowHeight*arrowLocation.y + gridRowHeight/2 );

  for (int i = 0; i < unit->numOfMovements(); ++i) { // For each queued movement in selected unit...
    // Rotate sprite and draw a(n)...
    // Upward arrow
    if ( unit->getMovement(i).x == 0 && unit->getMovement(i).y == -1 ) {
      arrowSprite.Move( 0, -gridRowHeight/2 );
      arrowSprite.SetRotation(270);
      window.Draw( arrowSprite );
      arrowSprite.Move( 0, -gridRowHeight/2 );
    }
    // Downward arrow
    else if ( unit->getMovement(i).x == 0 && unit->getMovement(i).y == 1 ) {
      arrowSprite.Move( 0, gridRowHeight/2 );
      arrowSprite.SetRotation(90);
      window.Draw( arrowSprite );
      arrowSprite.Move( 0, gridRowHeight/2 );
    }
    // Left arrow
    else if ( unit->getMovement(i).x == -1 && unit->getMovement(i).y == 0 ) {
      arrowSprite.Move( -gridColWidth/2, 0 );
      arrowSprite.SetRotation(0);
      window.Draw( arrowSprite );
      arrowSprite.Move( -gridColWidth/2, 0 );
    }
    // Right arrow
    else if ( unit->getMovement(i).x == 1 && unit->getMovement(i).y == 0 ) {
      arrowSprite.Move( gridColWidth/2, 0 );
      arrowSprite.SetRotation(180);
      window.Draw( arrowSprite );
      arrowSprite.Move( gridColWidth/2, 0 );
    }
    // Stopped
    else if ( unit->getMovement(i).isZero() ) {
      stopSprite.SetPosition( arrowSprite.GetPosition() );
      window.Draw( stopSprite );
    }
  }
}


void Level::drawCycle(int offset)
{
  //Edges of the cycle box
  int lowEdge = window.GetHeight() - bottom_offset / 5;
  int highEdge = window.GetHeight() - bottom_offset + bottom_offset / 5;

  //Edges of the cycle
  int highCycleEdge = highEdge + 20;
  int lowCycleEdge = lowEdge - 20;
  int leftCycleEdge = left_offset + 20;
  int rightCycleEdge = window.GetWidth() - right_offset - 20;


  //Number of cycles in the box
//  int numOfCycles = 2;
//(rightCycleEdge - leftCycleEdge) / cycleWidth / 2 * 2;

  //Width and height of one cycle pulse
  int cycleWidth = (rightCycleEdge - leftCycleEdge) / 2;

  //Whether currently in an upcycle or downcycle
  bool downCycle = 1;

  if(offset > 9)
    downCycle = 0;

  sf::Color backgroundColor = sf::Color(0, 0, 0, 128);
  sf::Color cycleColor = sf::Color(170, 0, 0);
  sf::Color arrowColor = sf::Color(0, 92, 9, 200);

  window.Draw(sf::Shape::Rectangle(left_offset,
                                   lowEdge,
                                   window.GetWidth() - right_offset,
                                   highEdge,
                                   backgroundColor));

  if(downCycle)
  {
    //High cycle on the left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth - offset * cycleWidth / 10,
                                highCycleEdge,
                                4, cycleColor));
    //High cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * 2 - offset * cycleWidth / 10,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth * 2,
                                highCycleEdge,
                                4, cycleColor));
    //Low cycle on left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * 1 - offset * cycleWidth / 10,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * 2 - offset * cycleWidth / 10,
                                lowCycleEdge,
                                4, cycleColor));
  }
  else
  {
    //Low cycle on the left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth - (offset - 10) * cycleWidth / 10,
                                lowCycleEdge,
                                4, cycleColor));
    //Low cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * 2 - (offset - 10) * cycleWidth / 10,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * 2,
                                lowCycleEdge,
                                4, cycleColor));
    //High cycle on right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * 2 - offset * cycleWidth / 10,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth * 3 - offset * cycleWidth / 10,
                                highCycleEdge,
                                4, cycleColor));
  }

  //Draw the vertical lines, conditioned for the disappearing and appearing lines
  for(int count = 1; count < 4; count++)
    if((downCycle && count < 3) || (!downCycle && count > 1))
      window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - offset * cycleWidth / 10,
                                  highCycleEdge,
                                  leftCycleEdge + cycleWidth * count - offset * cycleWidth / 10,
                                  lowCycleEdge,
                                  4, cycleColor));

  //Defines a triangle shape in SFML
  int x1 = ARROW_LENGTH * -0.4;
  int x2 = ARROW_LENGTH * 0.4;
  int y1 = -ARROW_HEIGHT * 0.2;
  int y2 = -ARROW_HEIGHT * 0.5;

  sf::Shape Triangle;

  //The position of the center of the arrow
  Triangle.SetPosition((leftCycleEdge + rightCycleEdge) / 2, lowEdge + 20);

  //The vertices of the arrow
  Triangle.AddPoint(x1, y1, arrowColor);
  Triangle.AddPoint(x2, y1, arrowColor);
  Triangle.AddPoint((x1 + x2) / 2, y2, arrowColor);

  window.Draw(Triangle);
}

void Level::drawBackground() {
  window.Draw(backgroundSprite);
}

void Level::highlightSelect() {

  if( !deafFrames ) {
    CellGroup* unit = activeGroup->getSelectedUnit();
    if(unit == 0)
      return;

    vector<FloatPair> groupLocations = unit->getScreenLocations();

    for (int i = 0; i < groupLocations.size(); ++i) {
      highlightSprite.SetPosition( groupLocations[i].x, groupLocations[i].y );
      window.Draw( highlightSprite );
    }
  }

}

void Level::requestDeafFrames( int requestedAmount ) {
  if( deafFrames < requestedAmount)
    deafFrames = requestedAmount;
}

/** ACCESSORS **/
CellGroup* Level::unitAtLocation ( Location loc ) {
  if( grid[0].find( loc) != grid[0].end() )
    return grid[0][loc];
  else
    return 0;
}

set<CellGroup*> Level::getFlaggedUnits () {
  return flaggedUnits;
}

string Level::nextLevel () {
  return destination;
}

int Level::done () {
  if( deafFrames ) return 0; // Level is not complete until animations finish
  return isDone;
}

int Level::getTopOffset()
{
  return top_offset;
}

int Level::getBottomOffset()
{
  return bottom_offset;
}

int Level::getRightOffset()
{
  return right_offset;
}

int Level::getLeftOffset()
{
  return left_offset;
}

int Level::getCyclesPerPeriod()
{
  return cyclesPerPeriod;
}

int Level::getWidth () {
  return width;
}

int Level::getHeight () {
  return height;
}
