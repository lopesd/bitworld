#define FULLSCREEN 0
#define LEVELFILE "levels/level_T1.bit"

int FPS = 30;

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
#include "classes/InfoBox.h"

using namespace std;

int main (void) {
  sf::Clock clock; //The whole thing crashes if I don't initialize a clock for some reason...

  // INITIALIZE LIBRARIES AND OBJECTS
  sf::RenderWindow window;
  window.SetFramerateLimit( FPS );
  window.UseVerticalSync( true );
  sf::VideoMode nativeResolution = sf::VideoMode::GetDesktopMode();

  if(FULLSCREEN)
    window.Create(nativeResolution, "BitWorld", sf::Style::Fullscreen,
                  sf::WindowSettings(24, 8, 4));
  else
    window.Create(sf::VideoMode(1280, 720), "BitWorld", sf::Style::Close,
                  sf::WindowSettings(24, 8, 4));

  ImageCache::LoadFromDirectory( "./images/" ); // Initialize image cache
//  Level level = LevelParser::parse (LEVELFILE, window);
  Level* level = new Level( LevelParser::Parse (LEVELFILE, window) );
  InfoBox* infoBox = new InfoBox(LEVELFILE, *level, window);

  // INITIALIZE INPUT VARIABLES
  Location   Lorder; // Input interpreted as location
  Direction  Dorder; // Input interperted as direction

  sf::Sprite test( ImageCache::GetImage("simple_bit.png") );
  float rotation = 0;

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
	  level->handleInput(Dorder);
	  break;
	case sf::Key::Right:
	  Dorder.x = 1;
	  Dorder.y = 0;
	  level->handleInput(Dorder);
	  break;
	case sf::Key::Up:
	  Dorder.x = 0;
	  Dorder.y = -1;
	  level->handleInput(Dorder);
	  break;
	case sf::Key::Down:
	  Dorder.x = 0;
	  Dorder.y = 1;
	  level->handleInput(Dorder);
	  break;
	case sf::Key::S:
	  Dorder.x = 0;
	  Dorder.y = 0;
	  level->handleInput(Dorder);
	  break;
	case sf::Key::Back:
	  level->handleInput(sf::Key::Back);
	  break;
	case sf::Key::Space:
	  level->handleInput(sf::Key::Space);
	  break;
        }

      if(Event.Type == sf::Event::MouseButtonReleased)
        if(Event.MouseButton.Button == sf::Mouse::Left) //left mouse click
          level->prepareInput(Event.MouseButton.X, Event.MouseButton.Y, 0); //the 0 flag indicates a left mouse click
	else if (Event.MouseButton.Button == sf::Mouse::Right) { //right mouse click
	  level->prepareInput(Event.MouseButton.X, Event.MouseButton.Y, 1); //the 1 flag indicates a right mouse click
	}
    }

    if( level->done() ) {
      cout << "Level is done and the new level should be " << level->nextLevel () << endl;
      string newLevel = level->nextLevel();
      level->destroy();
      level = new Level( LevelParser::Parse( newLevel.c_str(), window) );
cout << "TEST" << endl;
      infoBox = new InfoBox(newLevel.c_str(), *level, window);
    }

    window.Clear();

    level->draw(); //Draw level on window
    infoBox->draw();
    window.Display(); //Display window
  }

  return 0;
}

struct compareCellGroup {
  bool operator() ( CellGroup* first, CellGroup* second ) {
    return (first->getWeight() < second->getWeight());
  }
} compareCellGroup;
