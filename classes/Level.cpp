#include "Level.h"

#include <SFML/Graphics.hpp>

#include <iostream> //Remove later
#include <unistd.h> //For terminal version

#define TOP_OFFSET 50
#define BOTTOM_OFFSET 200
#define RIGHT_OFFSET 300
#define LEFT_OFFSET 300
#define ARROW_HEIGHT 50
#define ARROW_LENGTH 50
#define PI 3.14159265358979
Level::Level (sf::RenderWindow &newWindow, vector<ControlGroup*> c, vector<CellGroup*> u,
              int w, int h, int cpp)
:window(newWindow)
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
      cout << "Inserting pointer at location: " << locs.at(j).x << ", " << locs.at(j).y << endl;
      grid.insert( pair<Location, CellGroup*>( locs.at(j), units.at(i) ) );
    }
  }

  int count = 0;
  for ( map<Location, CellGroup*>::iterator i = grid.begin(); i != grid.end(); ++i, count++)
    cout << "Elements in the grid, " << count << ": " << i->first.x << " " << i->first.y << endl;

}

void Level::display ()
{
  drawBackground();

  drawGrid();

  drawUnits();

  highlightSelect();

  drawArrows();

//  drawCycle();
}

void Level::handleInput (Location loc)
{
  cout << "Location: " << loc.x << ", " << loc.y << endl;
  map<Location, CellGroup*>::iterator clickedUnit = grid.find( loc );

  if (clickedUnit != grid.end() )
  {
    cout << "There is a unit at that location." << endl;
    clickedUnit->second->controlGroup->handleInput ( clickedUnit->second );
  }
  else
  {
    cout << "NO UNIT at that location." << endl;
    CellGroup* nullPointer = 0;
    activeGroup->handleInput ( nullPointer );
  }
}

void Level::handleInput (Direction dir)
{
  activeGroup->handleInput (dir);
}

void Level::run ()
{
  //commit all movements, doing collision detection and stuff
  //cycle a certain number of times

  for (int i = 0; i < cyclesPerPeriod; ++i)
  {
    for (int j = 0; j < units.size(); ++j)
      units.at(j)->upCycle ();

    updateGrid();
    display();
    window.Display();
    usleep(100000);
  }
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

void Level::drawUnits()
{
  Location loc;

  sf::Image bit;

  bit.LoadFromFile("Normal1.png");
  sf::Sprite sBit(bit);
  sBit.Resize(gridColWidth, gridRowHeight);

  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
    {
      loc.x = i;
      loc.y = j;

      if ( grid.find( loc ) != grid.end() )
      {
        sBit.SetPosition(LEFT_OFFSET + gridColWidth * i, TOP_OFFSET + gridRowHeight * j);
        window.Draw(sBit);
      }
    }
}

void Level::prepareInput(int x, int y)
{
  Location Lorder;

  if(y > TOP_OFFSET && y < window.GetHeight() - BOTTOM_OFFSET &&
     x > LEFT_OFFSET && x < window.GetWidth() - RIGHT_OFFSET)
  {
    Lorder.x = (x - LEFT_OFFSET) / gridColWidth;
    Lorder.y = (y - TOP_OFFSET) / gridRowHeight;
  }

  handleInput(Lorder);
}

void Level::highlightSelect()
{
  vector<Location> groupLocations;
  CellGroup* unit;
  unit = activeGroup->getSelectedUnit();

  if(unit == 0)
    return;

  groupLocations = unit->getLocations();

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

    Triangle.SetPosition((LEFT_OFFSET + ARROW_LENGTH * (count + 0.5)),
                         (window.GetHeight() - BOTTOM_OFFSET + ARROW_HEIGHT * 0.5));

    Triangle.AddPoint(x1, y1, darkBlue);
    Triangle.AddPoint(x2, y1, darkBlue);
    Triangle.AddPoint((x1 + x2) / 2, y2, darkBlue);

    switch(unit->getMovement(count).x)
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
}

void Level::drawUpArrow(int moveNumber)
{
}

void Level::drawDownArrow(int moveNumber)
{
}

void Level::drawRightArrow(int moveNumber)
{
}

void Level::drawLeftArrow(int moveNumber)
{
}

/*
void Level::drawCycle()
{
  int lowOffset = BOTTOM_OFFSET / 5;
  int highOffset = BOTTOM_OFFSET / 5;

  int height = BOTTOM_OFFSET - lowOffset - highOffset;
}
*/

void Level::drawBackground()
{
  sf::Image background;
  if(!background.LoadFromFile("background.png"))
  {
    cout << "Can't load the background." << endl;
  }

  sf::Sprite sBackground(background);
  sBackground.Resize(window.GetWidth(), window.GetHeight());

  window.Draw(sBackground);
}
