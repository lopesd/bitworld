#define FULLSCREEN 0
#define LEVELFILE "levels/sentinel.bit"

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

  //sf::Clock clock; //The whole thing crashes if I don't initialize a clock for some reason...

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
  int doNotQuit = 1;
  
  while (doNotQuit) {
    //  Level level = LevelParser::parse (LEVELFILE, window);
    Level* level = new Level( LevelParser::Parse (LEVELFILE, window) );
    //cout << "game over status is: " << level->getGameOver() << endl;
    InfoBox* infoBox = new InfoBox(LEVELFILE, *level, window);
    
    // INITIALIZE INPUT VARIABLES
    Location   Lorder; // Input interpreted as location
    Direction  Dorder; // Input interperted as direction
    
    //START SCREEN
    //sets up and displays start screen graphics
    int pressSpaceToStart = 0;
    sf::Event startingEvent;
    sf::Event endingEvent;
    sf::Sprite StartScreen(ImageCache::GetImage("Bitworld.png"));
    StartScreen.SetPosition(window.GetWidth()/2 - (StartScreen.GetSize().x/2) , StartScreen.GetSize().y/2);
    window.Clear(sf::Color(0,0,0));
    window.Draw(StartScreen);
    window.Display();
    
    //loop for start screen
    while (!pressSpaceToStart) {
      //cout << "press space to start" << endl;
      while(window.GetEvent(startingEvent)) {
	if (startingEvent.Type == sf::Event::Closed) { //in case user wants to close the window rather than playing
	  window.Close();
	  doNotQuit = 0;
	  return 0;
	}
	else if(startingEvent.Type == sf::Event::KeyPressed) 
	  if(startingEvent.Key.Code == sf::Key::Space)  //if the user pressed space, then yeah
	    pressSpaceToStart = 1;
	  else if(startingEvent.Key.Code == sf::Key::Escape) {
	    window.Close();
	    doNotQuit = 0;
	    return 0;
	  } 
      }
    }
    
    
    // MAIN LOOP
    while (window.IsOpened()) {
      // EVENT HANDLING LOOP
      sf::Event Event;
      while (window.GetEvent(Event))
	{
	  // Window closed
	  if (Event.Type == sf::Event::Closed) {
	    window.Close();
	    doNotQuit = 1;
	    return 0;
	  }	
	  
	  if (Event.Type == sf::Event::KeyPressed)
	    switch(Event.Key.Code) {
	    case sf::Key::Escape:       // Escape key pressed
	      window.Close();
	      doNotQuit = 1;
	      break;
	      return 0;
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
      
      if(level->getGameOver() )//player lost
	{
	  level->destroy();
	  break;
	  //isDone = 1;
	}
      
      if( level->done() && !(level->getGameOver()) ) {
	cout << "Level is done and the new level should be " << level->nextLevel () << endl;
	string newLevel = level->nextLevel();
	
	Level* nextLevel = new Level( LevelParser::Parse( newLevel.c_str(), window) );
	level->transferUnits( nextLevel );

	level->destroy();
	delete level;
	level = nextLevel;
	delete infoBox;
	infoBox = new InfoBox( newLevel.c_str(), *level, window );
      }
      
      window.Clear();
      
      level->draw(); //Draw level on window
      infoBox->draw();
      window.Display(); //Display window
    }
    
    // GAME OVER SCREEN LOGIC STUFF BEGINS HERE LOLZ
    
    pressSpaceToStart = 0;
    window.Clear();
    sf::Sprite GameOverScreen(ImageCache::GetImage("GameOver.png"));
    GameOverScreen.SetPosition(window.GetWidth()/2 - (GameOverScreen.GetSize().x/2) , GameOverScreen.GetSize().y/2);
    window.Clear(sf::Color(0,0,0));
    window.Draw(GameOverScreen);
    window.Display();
    while(!pressSpaceToStart){
      while(window.GetEvent(endingEvent)) {
	//cout << "game over screen" <<endl;
	if(endingEvent.Type == sf::Event::KeyPressed) {
	  if(endingEvent.Key.Code == sf::Key::Space) {
	    pressSpaceToStart = 1;
	    //cout << "space happened what do" <<endl;
	  }
	  else if(endingEvent.Key.Code == sf::Key::Escape) {
	    window.Close();
	    pressSpaceToStart = 1;
	    doNotQuit = 0;
	  }
	  
	}
      }
    }
  }
  
  return 0;
  
}

struct compareCellGroup {
  bool operator() ( CellGroup* first, CellGroup* second ) {
    return (first->getWeight() < second->getWeight());
  }
} compareCellGroup;

  
