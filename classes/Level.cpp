/** Level.cpp
 *  The level object contains all the information about the grid for a certain level.
 *  It receives and interprets input from the user and controls ControlGroups.
 *  It is also responsible for drawing the grid portion of the screen.
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
*/

#include "Level.h"

#include <SFML/Graphics.hpp>

#include <iostream> //Remove later
#include <unistd.h> //For usleep function

using namespace std;

#define TOP_OFFSET 50
#define BOTTOM_OFFSET 200
#define RIGHT_OFFSET 300
#define LEFT_OFFSET 300
#define ARROW_HEIGHT 50
#define ARROW_LENGTH 50
#define PI 3.14159265358979

Level::Level (sf::RenderWindow &newWindow, vector<ControlGroup*> c, vector<CellGroup*> u,
              int w, int h, int cpp)
: window(newWindow)
{
  controlGroups = c;
  activeGroup = c.at(0); // set active group to be the first in the list
  units = u;
  width = w;
  height = h;
  cyclesPerPeriod = cpp;

  updateGrid();

  gridRowHeight = (window.GetHeight() - TOP_OFFSET - BOTTOM_OFFSET) / height;
  gridColWidth = (window.GetWidth() - LEFT_OFFSET - RIGHT_OFFSET) / width;
  top_offset = 50;
  bottom_offset = 200;
  right_offset = 300;
  left_offset = 300;

  for (int i = 0; i < units.size(); ++i) {
    units[i]->setGridData( gridColWidth, gridRowHeight, TOP_OFFSET, LEFT_OFFSET );
  }

  backgroundSprite.SetImage( ImageCache::GetImage("Dark.jpg") );
  backgroundSprite.Resize(window.GetWidth(), window.GetHeight());

}

/*
Level::Level (sf::RendewrWindow &newWindow) : window(newWindow) {
  cout << "Warning -- level not initialized. Call level.init() to initialize" << endl;
  }*/

void Level::init (vector<ControlGroup*> c, vector<CellGroup*> u, int w, int h, int cpp) {
  cout << "Initializing level" << endl;
  controlGroups = c;
  activeGroup = c.at(0); // set active group to be the first in the list
  units = u;
  width = w;
  height = h;
  cyclesPerPeriod = cpp;

  updateGrid();

  gridRowHeight = (window.GetHeight() - TOP_OFFSET - BOTTOM_OFFSET) / height;
  gridColWidth = (window.GetWidth() - LEFT_OFFSET - RIGHT_OFFSET) / width;

  for (int i = 0; i < units.size(); ++i) {
    units[i]->setGridData( gridColWidth, gridRowHeight, TOP_OFFSET, BOTTOM_OFFSET );
  }

}

Level::Level (const Level& L) : window(L.window) {
  controlGroups = L.controlGroups;
  activeGroup = controlGroups.at(0);
  units = L.units;
  width = L.width;
  height = L.height;
  cyclesPerPeriod = L.cyclesPerPeriod;

  updateGrid();

  gridRowHeight = (window.GetHeight() - TOP_OFFSET - BOTTOM_OFFSET) / height;
  gridColWidth = (window.GetWidth() - LEFT_OFFSET - RIGHT_OFFSET) / width;
}

void Level::updateGrid () { // Clears and remakes the entire grid
  // Clear grid first
  grid.clear();

  vector<Location> locs;

  for (int i = 0; i < units.size(); ++i)
  {
    locs = units.at(i)->getLocations(); // Get all the locations of a cellGroup (could be more than 1)
    for (int j = 0; j < locs.size(); ++j)// Set all a unit's positions to pointers to itself
    {
      grid.insert( pair<Location, CellGroup*>( locs.at(j), units.at(i) ) );
    }
  }
}

void Level::handleInput (Location loc)
{
  map<Location, CellGroup*>::iterator clickedUnit = grid.find( loc );

  if (clickedUnit != grid.end() ) {
    clickedUnit->second->controlGroup->handleInput ( clickedUnit->second );
  }
  else {
    CellGroup* nullPointer = 0;
    activeGroup->handleInput ( nullPointer );
  }
}

void Level::handleInput (Direction dir) {
  activeGroup->handleInput (dir);
}

void Level::handleInput (sf::Key::Code keyPressed)
{
  activeGroup->handleInput (keyPressed);
}

void Level::run ()
{
  //Move units and animate CPU cycle
  for (int i = 0; i < cyclesPerPeriod; i++)
  {
    //Move units
    for (int j = 0; j < units.size(); ++j)
      units.at(j)->upCycle ();
    updateGrid();
    display();

    //Animate CPU cycle
    for(int offset = 0; offset < 20; offset++)
    {
      drawCycle(offset);
      usleep(40000);
      window.Display();
    }
  }
}

void Level::display ()
{
  window.Clear();
  drawBackground();

  drawArrows();
  drawGrid();
  drawUnits();

  highlightSelect();

  drawCycle(0);
}

void Level::drawGrid()
{
  sf::Color gridColor(sf::Color(40, 40, 140, 0));
  sf::Color gridOutlineColor(sf::Color(100, 100, 240, 0));

  sf::Shape horLine;
  sf::Shape vertLine;
  int addLength;
  int addHeight;

  for(float scale = 0.05; scale >= 0; scale -= 0.01)
  {
    gridColor += sf::Color(0, 0, 0, 25);
    addLength = scale * (window.GetWidth() - LEFT_OFFSET - RIGHT_OFFSET) - 10;

    horLine = sf::Shape::Line(LEFT_OFFSET - addLength, TOP_OFFSET,
                            window.GetWidth() - RIGHT_OFFSET + addLength, TOP_OFFSET,
                            4, gridColor,
                            1, gridOutlineColor);

    window.Draw(horLine);

    for(int row = 0; row < height; row++)  //Draw horizontal lines
    {
      horLine.Move(0, gridRowHeight);
      window.Draw(horLine);
    }

    horLine.Move(0, -1 * gridRowHeight * height);

    addHeight = scale * (window.GetHeight() - TOP_OFFSET - BOTTOM_OFFSET) - 10;
    vertLine = sf::Shape::Line(LEFT_OFFSET, TOP_OFFSET - addHeight,
                               LEFT_OFFSET, window.GetHeight() - BOTTOM_OFFSET + addHeight,
                               4, gridColor,
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

void Level::prepareInput(int x, int y, int isRightClick) {
  Location Lorder;

  if(y > TOP_OFFSET && y < window.GetHeight() - BOTTOM_OFFSET &&
			   x > LEFT_OFFSET && x < window.GetWidth() - RIGHT_OFFSET) {
    Lorder.x = (x - LEFT_OFFSET) / gridColWidth;
    Lorder.y = (y - TOP_OFFSET)  / gridRowHeight;
  }

  if (!isRightClick)
    handleInput(Lorder);
  else {
    CellGroup* unit = activeGroup->getSelectedUnit();
    if (unit) {
      Direction Dorder;
      FloatPair middle = unit->getPathHead();
      
      cout << middle.x - Lorder.x << ", " << middle.y - Lorder.y << endl;
      int xdir = middle.x - Lorder.x;
      int ydir = middle.y - Lorder.y;

      if ( ((xdir == 1 || xdir == -1) && ydir == 0) || ((ydir == 1 || ydir == -1) && xdir == 0) ) {
	Dorder.x = Lorder.x - middle.x;
	Dorder.y = Lorder.y - middle.y;
	handleInput(Dorder);
      }
    }
    
  }
}

void Level::drawArrows()
{
  vector<Location> groupLocations;
  CellGroup* unit;
  unit = activeGroup->getSelectedUnit();

  if(unit == 0)
    return;

  sf::Color darkBlue = sf::Color(0, 0, 205, 190);

  for(int count = 0; count < unit->numOfMovements(); count++)
  {
    window.Draw(sf::Shape::Rectangle(LEFT_OFFSET + ARROW_LENGTH * count,
                                     window.GetHeight() - BOTTOM_OFFSET,
                                     LEFT_OFFSET + ARROW_LENGTH * (count + 1),
                                     window.GetHeight() - BOTTOM_OFFSET + ARROW_HEIGHT,
                                     sf::Color(50, 50, 50, 100)));
    window.Draw(sf::Shape::Rectangle(LEFT_OFFSET + ARROW_LENGTH * (count + 0.3),
                                     window.GetHeight() - BOTTOM_OFFSET + ARROW_HEIGHT * 0.3,
                                     LEFT_OFFSET + ARROW_LENGTH * (count + 0.7),
                                     window.GetHeight() - BOTTOM_OFFSET + ARROW_HEIGHT * 0.7,
                                     darkBlue));
    int x1 = ARROW_LENGTH * -0.4;
    int x2 = ARROW_LENGTH * 0.4;
    int y1 = -ARROW_HEIGHT * 0.2;
    int y2 = -ARROW_HEIGHT * 0.5;

    sf::Shape Triangle;

    Triangle.SetPosition( (LEFT_OFFSET + ARROW_LENGTH * (count + 0.5)),
                         (window.GetHeight() - BOTTOM_OFFSET + ARROW_HEIGHT * 0.5));

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

  // DRAW ON GRID ARROWS
  // Create and position arrow sprite
  FloatPair arrowLocation = unit->getMiddle();
  sf::Sprite arrowSprite;
  arrowSprite.SetImage( ImageCache::GetImage( "arrow2.png" ) );
  arrowSprite.Resize( gridColWidth, gridRowHeight );
  arrowSprite.SetPosition( left_offset + gridColWidth*arrowLocation.x + gridColWidth/2,
                           top_offset + gridRowHeight*arrowLocation.y + gridRowHeight/2);
  arrowSprite.SetCenter( arrowSprite.GetSize() / 2.f );

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
  }
  
}


void Level::drawCycle(int offset)
{
  //Edges of the cycle box
  int lowEdge = window.GetHeight() - BOTTOM_OFFSET / 5;
  int highEdge = window.GetHeight() - BOTTOM_OFFSET + BOTTOM_OFFSET / 5;

  //Edges of the cycle
  int highCycleEdge = highEdge + 20;
  int lowCycleEdge = lowEdge - 20;
  int leftCycleEdge = LEFT_OFFSET + 20;
  int rightCycleEdge = window.GetWidth() - RIGHT_OFFSET - 20;

  //Width and height of one cycle pulse
  int cycleWidth = lowCycleEdge - highCycleEdge;

  //Number of cycles in the box
  int numOfCycles = (rightCycleEdge - leftCycleEdge) / cycleWidth / 2 * 2;

  //Whether currently in an upcycle or downcycle
  bool upCycle = 1;

  if(offset > 9)
    upCycle = 0;

  sf::Color backgroundColor = sf::Color(0, 0, 0);
  sf::Color cycleColor = sf::Color(255, 0, 0);
  sf::Color arrowColor = sf::Color(0, 0, 205, 190);

  window.Draw(sf::Shape::Rectangle(LEFT_OFFSET,
                                   lowEdge,
                                   window.GetWidth() - RIGHT_OFFSET,
                                   highEdge,
                                   backgroundColor));

  if(upCycle)
  {
    //High cycle on the left is disappearing
    window.Draw(sf::Shape::Line(leftCycleEdge,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth - offset * cycleWidth / 10,
                                highCycleEdge,
                                4, cycleColor));
    //High cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * numOfCycles - offset * cycleWidth / 10,
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
                                leftCycleEdge + cycleWidth - (offset - 10) * cycleWidth / 10,
                                lowCycleEdge,
                                4, cycleColor));
    //Low cycle on the right is appearing
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * numOfCycles - (offset - 10) * cycleWidth / 10,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * numOfCycles,
                                lowCycleEdge,
                                4, cycleColor));
  }

  //Draw the up cycles, conditioned for the appearing upper right line
  for(int count = 2; count < numOfCycles || (!upCycle && count < numOfCycles + 1); count += 2)
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - offset * cycleWidth / 10,
                                highCycleEdge,
                                leftCycleEdge + cycleWidth * (count + 1) - offset * cycleWidth / 10,
                                highCycleEdge,
                                4, cycleColor));

  //Draw the down cycles, conditioned for the disappearing lower left line
  for(int count = numOfCycles - 1; count > 1 || (count > 0 && upCycle); count -= 2)
    window.Draw(sf::Shape::Line(leftCycleEdge + cycleWidth * count - offset * cycleWidth / 10,
                                lowCycleEdge,
                                leftCycleEdge + cycleWidth * (count + 1) - offset * cycleWidth / 10,
                                lowCycleEdge,
                                4, cycleColor));

  //Draw the vertical lines, conditioned for the disappearing and appearing lines
  for(int count = 1; count < numOfCycles + 2; count++)
    if((upCycle && count < numOfCycles + 1) || (!upCycle && count > 1))
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

void Level::highlightSelect()
{
  CellGroup* unit = activeGroup->getSelectedUnit();
  if(unit == 0)
    return;

  vector<Location> groupLocations = unit->getLocations();

  sf::Color highlightColor = sf::Color(255, 140, 0);

  sf::Shape square;

  int vertexXLeft;
  int vertexXRight;
  int vertexYUp;
  int vertexYDown;

  for(vector<Location>::iterator it = groupLocations.begin(); it != groupLocations.end(); it++)
  {
    vertexXLeft = LEFT_OFFSET + it->x * gridColWidth + 3;
    vertexXRight = LEFT_OFFSET + (it->x + 1) * gridColWidth - 3;
    vertexYUp = TOP_OFFSET + it->y * gridRowHeight + 3;
    vertexYDown = TOP_OFFSET + (it->y + 1) * gridRowHeight - 3;

    square = sf::Shape::Rectangle(vertexXLeft, vertexYUp, vertexXRight, vertexYDown,
             sf::Color(0, 0, 0, 0), 3, highlightColor);
    window.Draw(square);
  }
}
