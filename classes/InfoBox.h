/** InfoBox.h
 *  The InfoBox displays informatio on the right side of the screen that is pertinate to that
 *  particular level's rules, obstacles, and victory conditions
*/

#include <string>
#include <SFML/Graphics.hpp>

#include "Level.h"

using namespace std;

class InfoBox
{
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
    string title;
    vector<string> infoText; //Contains level information
    sf::Font infoTextFont;

    double titleCharacterWidth;
    int titleCharsPerLine;
    double infoCharacterWidth;
    int infoCharsPerLine;

    Level& level;
    sf::RenderWindow& window;
};
