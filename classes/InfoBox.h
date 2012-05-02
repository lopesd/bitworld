/** InfoBox.h
 *  The InfoBox displays informatio on the right side of the screen that is pertinate to that
 *  particular level's rules, obstacles, and victory conditions
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#include <string>
#include <SFML/Graphics.hpp>

#include "Level.h"

class InfoBox {

  public:
    InfoBox(const char *filename, Level& level, sf::RenderWindow& window);
    void draw();

  private:
    //Utility Functions
    void drawHeader();
    void drawBoxes();
    void drawText();

    int topBorder;
    int bottomBorder;
    int leftBorder;
    int rightBorder;
    int xCenter;
    int yCenter;

    sf::String renderTitle;
    std::string title;
    std::vector<std::string> infoText; //Contains level information
    sf::Font infoTextFont;
    sf::Font infoTitleFont;

    double titleCharacterWidth;
    int titleCharsPerLine;
    double infoCharacterWidth;
    int infoCharsPerLine;

    Level& level;
    sf::RenderWindow& window;
};
