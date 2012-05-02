/** InfoBox.cpp
 *  The InfoBox displays informatio on the right side of the screen that is pertinate to that
 *  particular level's rules, obstacles, and victory conditions
 *  Written for Bitworld by: David Lopes, Casey O'Meilia, Catherine Carothers, Mark Riehm
 */

#define SPACING 40
#define HEADER_HEIGHT 70
#define TITLEFONTSIZE 45
#define TEXTFONTSIZE 18

#include "InfoBox.h"
#include "LevelParser.h"

using namespace std;

InfoBox::InfoBox(const char* filename, Level& newLevel, sf::RenderWindow& newWindow)
:level(newLevel), window(newWindow)
{
  if(!infoTextFont.LoadFromFile("fonts/Courier.ttf", TEXTFONTSIZE))
  {
    cout << "Can't load the infobox font file." << endl;
    exit(0);
  }
  if(!infoTitleFont.LoadFromFile("fonts/Courier.ttf", TITLEFONTSIZE))
  {
    cout << "Can't load the infobox font file." << endl;
    exit(0);
  }

  //Import text from level file
  infoText = LevelParser::getInfoText(filename);

  //Set up the dimensinos of the boxes
  topBorder = SPACING;
  leftBorder = window.GetWidth() - 400;
  bottomBorder = window.GetHeight() - 200;
  rightBorder = window.GetWidth() - SPACING;
  xCenter = (leftBorder + rightBorder) / 2.0;
  yCenter = (topBorder + bottomBorder) / 2.0;

  //Determine the spacing of the title characters
  renderTitle = sf::String(infoText[0].c_str(), infoTextFont, TITLEFONTSIZE);
  titleCharacterWidth = renderTitle.GetCharacterPos(1).x - renderTitle.GetCharacterPos(0).x;
  titleCharsPerLine = (rightBorder - leftBorder) / titleCharacterWidth;

  //Add necessary spaces to the beginning of the title to center it
  title += infoText[0];

  renderTitle = sf::String(title.c_str(), infoTitleFont, TITLEFONTSIZE);
}

void InfoBox::draw()
{
  drawBoxes();
  drawHeader();
  drawText();
}

void InfoBox::drawBoxes()
{
  sf::Shape infoBoxBox = sf::Shape::Rectangle(leftBorder, topBorder,
                                              rightBorder, bottomBorder,
                                              sf::Color(128, 128, 128),
                                              2, sf::Color(212, 175, 55));
  sf::Shape infoBoxHeader = sf::Shape::Rectangle(leftBorder, topBorder,
                                                 rightBorder, topBorder + HEADER_HEIGHT,
                                                 sf::Color(30, 30, 30),
                                                 2, sf::Color(212, 175, 55));
  window.Draw(infoBoxBox);
  window.Draw(infoBoxHeader);
}

void InfoBox::drawHeader()
{
  int titleOffset = ((rightBorder - leftBorder) - titleCharacterWidth * title.size()) / 2;
  renderTitle.SetPosition(leftBorder + titleOffset, topBorder);
  window.Draw(renderTitle);
}

void InfoBox::drawText()
{
  for(int count = 1; count < infoText.size(); count++)
  {
    sf::String renderInfo = sf::String(infoText[count].c_str(), infoTextFont, TEXTFONTSIZE);
    renderInfo.SetColor(sf::Color::Black);
    renderInfo.SetPosition(leftBorder, topBorder + TEXTFONTSIZE * count + 60);
    window.Draw(renderInfo);
  }
}
