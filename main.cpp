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

int main (void) {

  // SET PARAMETERS, INITIALIZE VARIABLES
  char       cinp;   // The raw character input in the terminal version
  int        inp;    // The raw integer input in the terminal version
  Location   Lorder; // Input interpreted as location
  Direction  Dorder; // Input interperted as direction

  // INITIALIZE IMAGES AND LEVELS
  Cell cell  ( 0, 0 );
  Cell cell2 ( 1, 1 );
  Cell cell3 ( 1, 0 );
  Cell cell4 ( 0, 1 );
  CellGroup group1 (&cell);
  CellGroup group2 (&cell2);
  CellGroup group3 (&cell3);
  CellGroup group4 (&cell4);

  //vector<Cell*> cellVector;
  /*llVector.push_back( &cell  );
  cellVector.push_back( &cell2 );
  cellVector.push_back( &cell3 );
  cellVector.push_back( &cell4 );*/
  //CellGroup cellGroup ( cellVector );
  vector<CellGroup*> groupVector;
  groupVector.push_back ( &group1 );
  groupVector.push_back ( &group2 );
  groupVector.push_back ( &group3 );
  groupVector.push_back ( &group4 );
  UserControlGroup usg (groupVector);
  vector<ControlGroup*> cgv;
  cgv.push_back (&usg);

  sf::RenderWindow window;
  sf::VideoMode nativeResolution = sf::VideoMode::GetDesktopMode();

  if(FULLSCREEN)
    window.Create(nativeResolution, "BitWorld", sf::Style::Fullscreen,
                  sf::WindowSettings(24, 8, 4));
  else
    window.Create(sf::VideoMode(1280, 720), "BitWorld", sf::Style::Close,
                  sf::WindowSettings(24, 8, 4));

  Level level (window, cgv, groupVector);

  // INITIAL DRAWING
/*
  // MAIN LOOP
  while (1) {

    // POLL FOR EVENT
    cout << "Click or direction? [c/d]";
    cin  >> cinp;

    if (cinp == 'c') {
      cout << "X: ";
      cin >> inp;
      Lorder.x = inp;
      cout << "Y: ";
      cin >> inp;
      Lorder.y = inp;
      level.handleInput (Lorder);
    }

    if (cinp == 'd') {
      cout << "X: ";
      cin >> inp;
      Dorder.x = inp;
      cout << "Y: ";
      cin >> inp;
      Dorder.y = inp;
      level.handleInput (Dorder);
    }

    if (cinp == 'r') {
      level.run ();
    }

    // HANDLE EVENT

    // CYCLE LEVEL EVENTS

    // DISPLAY LEVEL
  }
*/

  //Load the Background
  sf::Image background;
  if(!background.LoadFromFile("background.png"))
  {
    cout << "Can't load the background." << endl;
  }

  sf::Sprite sBackground(background);
  sBackground.Resize(window.GetWidth(), window.GetHeight());

  // MAIN LOOP

  window.Clear();

  while (window.IsOpened())
  {
    // EVENT HANDLING LOOP
    sf::Event Event;
    while (window.GetEvent(Event))
    {
      // Window closed
      if (Event.Type == sf::Event::Closed)
        window.Close();

      // Escape key pressed
      if (Event.Type == sf::Event::KeyPressed)
        switch(Event.Key.Code)
        {
          case sf::Key::Escape:
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
          case sf::Key::R:
            level.run();
        }

      if(Event.Type == sf::Event::MouseButtonReleased)
        if(Event.MouseButton.Button == sf::Mouse::Left)
          level.prepareInput(Event.MouseButton.X, Event.MouseButton.Y);
    }

    window.Draw(sBackground);
    level.display ();
    window.Display();
  }

  return 0;

}