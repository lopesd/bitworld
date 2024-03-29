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
#include <algorithm> //For the find in vector function
#include <cstring>

using namespace std;

#define ARROW_HEIGHT 50
#define ARROW_LENGTH 50

extern int FPS;

// Used to sort the units vector, compare pointers to CellGroup
struct pointerCompare2 {
  bool operator() (CellGroup* first, CellGroup* second) {
    return ( first->getWeight() < second->getWeight() );
  }
} pointerCompare2;


/** CONSTRUCTORS **/
Level::Level (sf::RenderWindow &newWindow, vector<ControlGroup*> c, vector<CellGroup*> u, vector<Gate*> g,
              int w, int h, int cpp)
: window(newWindow)
{
  // TAKE IN UNITS AND CONTROL GROUPS
  units = u; // Take in units

  controlGroups = c; // Take in Control Groups
  for( int i = 0; i < controlGroups.size(); ++i ) // Tell controlGroups to recognize me as their level overlord
    controlGroups[i]->setLevel( this );

  gates = g; // Take in gates
  for( int i = 0; i < gates.size(); ++i )
    gates[i]->level = this;

  // SET GRID DATA
  width = w;
  height = h;
  cyclesPerPeriod = cpp;
  cyclesToRun = 0;
  partOfCycle = 0;
  isDone = 0;

  grid = doubleBufferGrid;
  resetGrid();
  future = 1;

  // Calculate location of grid to be drawn
  FloatPair center;
  center.x = (float)(window.GetWidth() - 400)/2;
  center.y = (float)(window.GetHeight() - 150)/2;
  gridRowHeight = 40;
  gridColWidth  = 40;

  left_offset = center.x - (float)(width)/2*gridColWidth;
  top_offset  = center.y - (float)(height)/2*gridRowHeight;
  bottom_offset = window.GetHeight() - top_offset - gridRowHeight*height;
  right_offset = left_offset;
  deafFrames = 0;
  cycleOffset = 0;

  // Set the grid data for all my units
  for (int i = 0; i < units.size(); ++i)
    units[i]->setGridData( gridColWidth, gridRowHeight, top_offset, left_offset );
  for (int i = 0; i < gates.size(); ++i)
    gates[i]->setGridData( gridColWidth, gridRowHeight, top_offset, left_offset );

  // Set sprite stuff
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

  // Miscellaneous values
  activeGroupIndex = 0;
  activeGroup = controlGroups[0]; // set active group to be the first in the list
  activeGroup->startTurn();

  gameOver = 0;
}

// Copy constructor must be defined mainly to pass control appropriately
Level::Level (const Level& L) : window(L.window) {
  cout << "In level copy constructor." << endl;
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
  gameOver = 0;
  partOfCycle = 0;

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

// A "custom destructor" -- deletes everything that the level points to
void Level::destroy () {
  for( int i = 0; i < units.size(); ++i ) {
    if( unitsToTransfer.find(units[i]) == unitsToTransfer.end() ) // If the unit is not being transferred to another level
      delete units[i];
  }
  for( int i = 0; i < controlGroups.size(); ++i ) {
    delete controlGroups[i];
  }
  for( int i = 0; i < gates.size(); ++i ) {
    delete gates[i];
  }
}

// Clears and remakes the entire grid
void Level::resetGrid () {
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
// Take user input in terms of a location on the screen, and interprets it as a click on its grid
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

// INPUT HANDLERS
void Level::handleInput (Location loc) {

  if( deafFrames ) return; // Ignore input if animations are happening

  map<Location, CellGroup*>::iterator clickedUnit = grid[0].find( loc );

  if (clickedUnit != grid[0].end() ) { // A unit was clicked
    activeGroup->handleInput ( clickedUnit->second );
  }
  else { // An empty cell was clicked
    CellGroup* nullPointer = 0;
    activeGroup->handleInput ( nullPointer );
  }
}

// Defer decision to control groups
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

// Run a period
void Level::runPeriod () {
  cyclesToRun = cyclesPerPeriod;
}

// Run a cycle
void Level::runCycle () {

  requestDeafFrames( 1 ); // at least one frame of silence.

  if( partOfCycle == 0 ) {
    runUpCycle();
    ++partOfCycle;
    requestDeafFrames( FPS/2 );
  } else if( partOfCycle == 1 ) {
    runDownCycle();
    ++partOfCycle;
    requestDeafFrames( FPS/2 );
  } else if( partOfCycle == 2 ) {
    //cout << "done with cycle. " << endl;
    --cyclesToRun;
    partOfCycle = 0;
  }

}

// Run the part of the cycle in which unit movement occurs
void Level::runUpCycle () {

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
      vector<Location> myLocs = units[i]->getLocations();
      if( !flaggedUnits.empty() ) {
	Direction dir = ((WhiteBit*)units[i])->findMove( grid[future], flaggedUnits ); //The white bit finds its desired move

	units[i]->issueMovementOrder( dir );
	for( int j = 0; j < myLocs.size(); ++j ) {
	  Location floc = myLocs[j] + dir;
	  if( grid[future].find( floc ) != grid[future].end() ) { //If there is a unit at the desired location
	    handleMerge( units[i], grid[future][floc], floc );
	  } else {
	    grid[future][floc] = units[i];
	  }
	}
      } else {
	for( int j = 0; j < myLocs.size(); ++j ) {
	  grid[future][myLocs[j]] = units[i];
	}
      }
    }

  //Move all units (only the ones that are free to move will move)
  for( int i = 0; i < units.size(); ++i )
    units[i]->upCycle ();
  //Switch present grid with future grid
  grid[0].clear(); //Clear old grid
  grid = grid + future;
  future = -future;

  runHighCycle();
  
}

// High cycles -- gates check if they should open
void Level::runHighCycle () {
  for( int i = 0; i < gates.size(); ++i ) {
    gates[i]->highCycle();
  }
}

// Down cycles -- events, such as pulsing
void Level::runDownCycle () {

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

  // Unit events
  for( int i = 0; i < units.size(); ++i ) {
    units[i]->downCycle();
  }


  runLowCycle();

}

// Nothing in here yet.
void Level::runLowCycle () {

}

//Recursive checking if the unit currently at myLoc will move (eg. no head-on collision)
int Level::willMove ( Location myLoc ) {
  if( grid[0].find( myLoc ) == grid[0].end() )
    cout << "could not find myLoc. OH my goodness gracious SEGFAULT" << endl;
  CellGroup* unit = grid[0][myLoc]; //the unit that owns a cell at the given location

  //always return 1 if the unit is a white bit and if there are flaggedUnits it should be chasing.
  if( strcmp( unit->type().c_str(), "WhiteBit" ) == 0 && !flaggedUnits.empty() ) {
    cout << "It is a moving whitebit!";
    return 1;
  }

  Direction myDir = unit->getMovement(0);
  Location fLoc = myLoc + myDir; //My desired future location
  if( (fLoc.x < 0) || (fLoc.y < 0) || (fLoc.x >= width) || (fLoc.y >= height) ) return 0; //Future position is off grid; do not move
  if( (myDir.isZero()) || (grid[0].find(fLoc) == grid[0].end()) ) {
    return 1;   //There is no one there (or I am stopped); I go (there may still be an absorption)
  }
  else { //If there is someone there...
    if( (fLoc + grid[0][fLoc]->getMovement(0) ) == myLoc ) return 0; //unit wants to move to where I am; head-on
    if( strcmp(grid[0][fLoc]->type().c_str(), "WhiteBit")==0 && !flaggedUnits.empty() ) return 1; //moving white bit
    if( grid[0][fLoc]->getMovement(0).isZero() )           return 0; //unit wants to remain where it is; head-on
    return( willMove(fLoc) );                                        //I am free to move only if my future position clears up.
  }
}

// Handles a merge or absorption between two units who move to the same location
void Level::handleMerge ( CellGroup* unit1, CellGroup* unit2, Location loc ) {

  //cout << "Merge Detected at " << loc << " between a " << unit1->type() << " and a " << unit2->type() << endl;

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
  if( activeGroup->getSelectedUnit() == unitToDie ) {
    activeGroup->clearSelection();
  }
  if( flaggedUnits.find(unitToDie) != flaggedUnits.end() ) flaggedUnits.erase( flaggedUnits.find(unitToDie) );
  unitToDie->controlGroup->forfeit(unitToDie);
  delete unitToDie;                 //Finally, deallocate him
}

// Unit is flagged by a pulser or sentinel
void Level::flagUnit ( CellGroup* unitToFlag ) {
  flaggedUnits.insert( unitToFlag );
}

// Handle a gate opening
void Level::openGate ( Gate* gate ) {
  isDone = 1;
  destination = gate->destination();
  unitsToTransfer = gate->getUnitsToTransfer();
  gateDestTag = gate->destinationTag;
}

// Transfer units from one gate to the next level's gate
void Level::transferUnits ( Level* newLevel ) {

  int locCounter = 0;
  Gate* destGate = newLevel->gateWithTag( gateDestTag );
  for( set<CellGroup*>::iterator it = unitsToTransfer.begin(); it != unitsToTransfer.end(); ++it ) {
    (*it)->setLocation( destGate->getLocations()[locCounter++] );
    newLevel->take( *it );

    for( int i = 0; i < units.size(); ++i ) { //remove the pointer from the units vector
      if( *it == units[i] ) {
	units.erase( units.begin() + i );
	break;
      }
    }

  }

  gameOver = 0;
  isDone = 0;
  cyclesToRun = 0;
  partOfCycle = 0;
}

// Take a unit into level
void Level::take ( CellGroup* unit ) {
  units.push_back( unit );
  unit->setGridData( gridColWidth, gridRowHeight, top_offset, left_offset );

  // Pass unit to player control. Assume all units transferred must have been the user's.
  for( int i = 0; i < controlGroups.size(); ++i ) {
    if( controlGroups[i]->getPlayer() ) {
      controlGroups[i]->take( unit );
    }
  }

  // Insert the unit in the grid
  for( int i = 0; i < unit->getLocations().size(); ++i )
    grid[0].insert( pair<Location, CellGroup*> ( unit->getLocations()[i], unit ) );

  // Resort grid so that drawing occurs correctly
  sort( units.begin(), units.end(), pointerCompare2 );
}

/* DRAWING */
void Level::draw() {

  // deafFrames is how long the level will ignore user input. We decrease the count by 1 each frame.
  if( deafFrames ) --deafFrames;
  if( deafFrames == 0 ) {
    // Perform some game logic

    //Kill any units that must die
    if( !unitsToDie.empty() ) {
      for( set<CellGroup*>::iterator i = unitsToDie.begin(); i != unitsToDie.end(); ++i )
	killUnit( *i );
      unitsToDie.clear();

      // If a unit has died, check to see if that was the last player's unit
      for( int i = 0; i < controlGroups.size(); ++i ) {
	if(controlGroups[i]->getPlayer() && !(controlGroups[i]->getUnitsSize()) ) {
	  gameOver = 1;
	  isDone = 1;
	}
      }
    }

    if( cyclesToRun != 0 && !done() ) { // run cycles if we still need to
      //cout << "Running part of a cycle because cyclesToRun is " << cyclesToRun << endl;
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
  drawGates();
  highlightSelect();
  drawUnits();
  drawAnimations();
  drawArrows();
  drawCycle();
}

// Draw the lines of the grid
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
                            left_offset + gridColWidth * width + addLength, top_offset,
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

// Draw each othe units
void Level::drawUnits() {
  for (int i = 0; i < units.size(); ++i)
    units[i]->draw( window );
}

// Draw each of the gates
void Level::drawGates() {
  for( int i = 0; i < gates.size(); ++i )
    gates[i]->draw( window );
}

// Draw each of the animations, checking to see if they have finished
void Level::drawAnimations() {
  for( int i = 0; i < animations.size(); ) {
    animations[i].draw( window );
    if( animations[i].isDone() )
      animations.erase( animations.begin() + i );
    else
      ++i;
  }
}

// Draw arrows on grid
void Level::drawArrows() {
  // Draw arrows for every unit of the active group, if it is a user's group
  if( activeGroup->getPlayer() )
    for( int i = 0; i < activeGroup->getUnitsSize(); ++i ) {
      CellGroup* unit = activeGroup->getUnits()[i];
      int notSelected = !( unit == activeGroup->getSelectedUnit() );

      if(unit == 0)
	return;

      // DRAW ON GRID ARROWS
      // Create and position arrow sprite
      FloatPair arrowLocation = unit->getMiddle();
      arrowSprite.SetPosition( left_offset + gridColWidth*arrowLocation.x + gridColWidth/2,
			       top_offset + gridRowHeight*arrowLocation.y + gridRowHeight/2 );
      if( notSelected ) {
	arrowSprite.SetColor( sf::Color( 0, 0, 0, 100) );
	stopSprite.SetColor( sf::Color( 0, 0, 0, 100) );
      } else {
	arrowSprite.SetColor( sf::Color( 255, 255, 255, 255 ) );
	stopSprite.SetColor( sf::Color( 255, 255, 255, 255 ) );
      }
	

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
}

// Draw CPU cycle
void Level::drawCycle()
{
  //Edges of the cycle box
  int lowEdge = window.GetHeight() - 25;
  int highEdge = window.GetHeight() - 175;

  //Edges of the cycle
  int highCycleEdge = highEdge + 20;
  int lowCycleEdge = lowEdge - 20;
  int leftCycleEdge = 40;
  int rightCycleEdge = window.GetWidth() - 40;

  //Width and height of one cycle pulse
  int cycleWidth = lowCycleEdge - highCycleEdge;

  //Number of cycles in the box
  int numOfCycles = (rightCycleEdge - leftCycleEdge) / cycleWidth / 2 * 2;
  leftCycleEdge = (window.GetWidth() - numOfCycles * cycleWidth) / 2;
  rightCycleEdge = leftCycleEdge + numOfCycles * cycleWidth;

  //Whether currently in an upcycle or downcycle
  bool upCycle = 1;

  if(partOfCycle == 1 && cycleOffset < FPS / 2)
    cycleOffset++;
  if(partOfCycle == 2 && cycleOffset < FPS)
    cycleOffset++;
  if(partOfCycle == 0)
    cycleOffset = 0;

  if(cycleOffset > FPS / 2)
    upCycle = 0;

  sf::Color backgroundColor = sf::Color(0, 0, 0, 180);
  sf::Color cycleColor = sf::Color(0, 255, 0);
  sf::Color arrowColor = sf::Color(255, 0, 0);

  window.Draw(sf::Shape::Rectangle(leftCycleEdge - 20,
                                   lowEdge,
                                   rightCycleEdge + 20,
                                   highEdge,
                                   backgroundColor));

  if(upCycle)
  {
    //High cycle on the left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth - cycleOffset * cycleWidth / FPS * 2,
                                highCycleEdge,
                                4, cycleColor));
    //High cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * numOfCycles - cycleOffset * cycleWidth / FPS * 2,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth * numOfCycles,
                                highCycleEdge,
                                4, cycleColor));
  }
  else
  {
    //Low cycle on the left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth - (cycleOffset - FPS / 2) * cycleWidth / FPS * 2,
                                lowCycleEdge,
                                4, cycleColor));
    //Low cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * numOfCycles - (cycleOffset - FPS / 2) * cycleWidth / FPS * 2,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * numOfCycles,
                                lowCycleEdge,
                                4, cycleColor));
  }

  //Draw the up cycles, conditioned for the appearing upper right line
  for(int count = 2; count < numOfCycles || (!upCycle && count < numOfCycles + 1); count += 2)
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - cycleOffset * cycleWidth / FPS * 2,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth * (count + 1) - cycleOffset * cycleWidth / FPS * 2,
                                highCycleEdge,
                                4, cycleColor));

  //Draw the down cycles, conditioned for the disappearing lower left line
  for(int count = numOfCycles - 1; count > 1 || (count > 0 && upCycle); count -= 2)
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - cycleOffset * cycleWidth / FPS * 2,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * (count + 1) - cycleOffset * cycleWidth / FPS * 2,
                                lowCycleEdge,
                                4, cycleColor));

  //Draw the vertical lines, conditioned for the disappearing and appearing lines
  for(int count = 1; count < numOfCycles + 2; count++)
    if((upCycle && count < numOfCycles + 1) || (!upCycle && count > 1))
      window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - cycleOffset * cycleWidth / FPS * 2,
                                  highCycleEdge,
                                  leftCycleEdge + cycleWidth * count - cycleOffset * cycleWidth / FPS * 2,
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

// Draw level background
void Level::drawBackground() {
  window.Draw(backgroundSprite);
}

// Draw orange selection tile
void Level::highlightSelect() {

  if( !deafFrames ) {
    CellGroup* unit = activeGroup->getSelectedUnit();
    if(unit == 0) return;

    vector<FloatPair> groupLocations = unit->getScreenLocations();

    for (int i = 0; i < groupLocations.size(); ++i) {
      highlightSprite.SetPosition( groupLocations[i].x, groupLocations[i].y );
      window.Draw( highlightSprite );
    }
  }

}

// Add an animation to the animation vector
void Level::addAnimation ( Animation* anim ) {
  animations.push_back( *anim );
}

// Set deaf frames to AT LEAST the requested amount
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

Gate* Level::gateWithTag ( int tag ) {
  for( int i = 0; i < gates.size(); ++i )
    if( gates[i]->tag == tag )
      return gates[i];
}

set<CellGroup*> Level::getFlaggedUnits () {
  return flaggedUnits;
}

string Level::nextLevel () {
  return destination;
}

int Level::done () {
  if( deafFrames || partOfCycle ) return 0; // Level is not complete until animations finish
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

int Level::getGameOver() {
  if( deafFrames || partOfCycle ) return 0; // Level is not complete until animations finish
  return gameOver;
}

int Level::getCellWidth () {
  return gridColWidth;
}

int Level::getCellHeight () {
  return gridRowHeight;
}

int Level::destinationGate () {
  return gateDestTag;
}
