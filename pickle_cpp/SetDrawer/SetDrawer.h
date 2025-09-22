#ifndef SET_DRAWER_H
#define SET_DRAWER_H

#include <sstream>  // Include for string stream operations
#include <vector>
#include <iostream>
#include <string>
#include "../../include/led-matrix.h"  // Include RGB matrix types
#include "../../include/graphics.h"    // Include graphics types
#include "../GameState/GameState.h"
#include "../Player/Player.h"
#include "../Drawer/Drawer.h"
#include "../FontLoader/FontLoader.h"
#include "../SetHistoryText/SetHistoryText.h"
// #include "../TennisConstants/TennisConstants.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../GameTimer/GameTimer.h"
#include "../ClockDrawer/ClockDrawer.h"
#include "../ClockTimer/ClockTimer.h"

using namespace rgb_matrix;  // Make RGB matrix types available

#define SMALL_BEFORE  7
#define SMALL_BETWEEN 17
#define START_ROW     89 // 86 // 102324 move Set scores 3 rows down.
#define SET_START_ROW 92 // 071125  // wip Blue Set Box
#define LITTLE_FONT   "fonts/the_sets_font.bdf"


class SetDrawer {
 public:
    SetDrawer( RGBMatrix* canvas, GameState* gameState );
    SetDrawer( RGBMatrix* canvas, GameState* gameState, ClockTimer* clockTimer );
    SetDrawer();
    ~SetDrawer();
    std::string  cloaker( std::string stringToCloak, int sectionToCloak );
    void drawSets();
    void drawBlinkSets( int player);
    void drawTextOnCanvas( int x, int y, const Color& color, const std::string& text );
    void blankSets();
    void blinkPickleSets( int winning_team_number );
    void drawSetBox();
    void drawCenterDivider();
    void setClockTimer(ClockTimer* clockTimer);

    // Getter for set font height
    int getSetFontHeight() const;

 private: 
    const int FIXED_SET_WIDTH  = 17; // Adjust as needed based on your font size
    const int SET_SPACING      = 0;  // Space between sets
    const int OFFSET_FOR_ONE   = 2;  // Additional offset for the digit "1"
    const int OFFSET_FOR_SEVEN = 1;  // Additional offset for the digit "7"

    rgb_matrix::Font    _little_font;
    RGBMatrix*          _canvas;
    GameState*          _gameState; 
    SetHistoryText      _setHistoryText;
    ClockDrawer*        _clockDrawer;
    ClockTimer*         _clockTimer;
    
    // Store last box dimensions for center divider
    int _lastBoxX;
    int _lastBoxY;
    int _lastBoxWidth;
    int _lastBoxHeight;

    const int BOTTOM_RAISE = 3;   // bottom border lifted 3 px

    // Helper function to split a string by spaces into a vector of strings
    std::vector<std::string> splitString( const std::string& str, char delimiter = ' ' );

    int _getStringWidth(const std::string& text);
    void _drawPlayerSets(const std::vector<std::string>& sets, Color color, int y);
    void _drawSetsWithSpacing(std::string playerOneSetString, std::string playerTwoSetString);
    void _fillRectangle(int x_start, int y_start, int width, int height, const Color& color);
    void _drawPickleSets();
    void _calculateBoxDimensions();
    void _drawBoxBorders();
    void _drawTopBorder();
    void _drawBottomBorder();
    void _drawLeftBorder();
    void _drawRightBorder();
    void _drawCenterDividerDetailed();
    void _clearWinningSetScore( int winning_team_number );
};
#endif