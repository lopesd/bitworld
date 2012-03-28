#define FULLSCREEN 0

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "classes/structures.h"
#include "classes/Level.h"
#include "classes/Bit.h"
#include "classes/Cell.h"
#include "classes/ControlGroup.h"
#include "classes/UserControlGroup.h"
#include "classes/LevelParser.h"
#include "classes/ImageCache.h"

int main (void) {

  // DEPRECATED TERMINAL VERSION CODE
  /*
  // INITIALIZE IMAGES AND LEVELS
  Cell cell  ( 5, 6 );
  Cell cell2 ( 7, 7 );
  Cell cell3 ( 2, 3 );
  Cell cell4 ( 8, 2 );
  CellGroup group1 (cell);
  CellGroup group2 (cell2);
  CellGroup group3 (cell3);
  CellGroup group4 (cell4);

  Cell cell5 ( 1, 1 );
  Cell cell6 ( 1, 2 );
  Cell cell7 ( 2, 1 );
  Cell cell8 ( 2, 2 );
  vector<Cell> cellVector;
  cellVector.push_back( cell5 );
  cellVector.push_back( cell6 );
  cellVector.push_back( cell7 );
  cellVector.push_back( cell8 );
  CellGroup cellGroup ( cellVector );

  vector<CellGroup*> groupVector;
  groupVector.push_back (&cellGroup);
  groupVector.push_back ( &group1 );
  groupVector.push_back ( &group2 );
  groupVector.push_back ( &group3 );
  groupVector.push_back ( &group4 );
  UserControlGroup usg (groupVector);
  vector<ControlGroup*> cgv;
  cgv.push_back (&usg);
  */

  sf::Clock clock; //The whole thing crashes if I don't initialize a clock for some reason...

  // INITIALIZE LIBRARIES AND OBJECTS
  sf::RenderWindow window;
  sf::VideoMode nativeResolution = sf::VideoMode::GetDesktopMode();

  if(FULLSCREEN)
    window.Create(nativeResolution, "BitWorld", sf::Style::Fullscreen,
                  sf::WindowSettings(24, 8, 4));
  else
    window.Create(sf::VideoMode(1280, 720), "BitWorld", sf::Style::Close,
                  sf::WindowSettings(24, 8, 4));

  ImageCache::LoadFromDirectory( "./images/" ); // Initialize image cache
  Level level = LevelParser::parse ("levels/level_T1.bit", window);

  // INITIALIZE INPUT VARIABLES
  Location   Lorder; // Input interpreted as location
  Direction  Dorder; // Input interperted as direction

  // MAIN LOOP
  while (window.IsOpened())
  {
    // EVENT HANDLING LOOP
    sf::Event Event;
    while (window.GetEvent(Event))
    {
      // Window closed
      if (Event.Type == sf::Event::Closed)
        window.Close();

      if (Event.Type == sf::Event::KeyPressed)
        switch(Event.Key.Code) {
	case sf::Key::Escape:       // Escape key pressed
	  window.Close();
	  break;
	case sf::Key::Left:
	  Dorder.x = -1;
	  Dorder.y = 0;
	  level.handleInput(Dorder);
	  break;
	case sf::Key::Right:
	  Dorder.x = 1;
	  Dorder.y = 0;
	  level.handleInput(Dorder);
	  break;
	case sf::Key::Up:
	  Dorder.x = 0;
	  Dorder.y = -1;
	  level.handleInput(Dorder);
	  break;
	case sf::Key::Down:
	  Dorder.x = 0;
	  Dorder.y = 1;
	  level.handleInput(Dorder);
	  break;
	case sf::Key::Back:
	  level.handleInput(sf::Key::Back);
	  break;
	case sf::Key::R:
	  level.run();
        }
      
      if(Event.Type == sf::Event::MouseButtonReleased)
        if(Event.MouseButton.Button == sf::Mouse::Left) //left mouse click
          level.prepareInput(Event.MouseButton.X, Event.MouseButton.Y, 0); //the 0 flag indicates a left mouse click
	else if (Event.MouseButton.Button == sf::Mouse::Right) { //right mouse click
	  level.prepareInput(Event.MouseButton.X, Event.MouseButton.Y, 1); //the 1 flag indicates a right mouse click
	}
    }

    level.display (); //Draw level on window
    window.Display(); //Display window
  }

  return 0;
}
