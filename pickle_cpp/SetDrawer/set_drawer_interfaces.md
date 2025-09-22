# https://chatgpt.com/share/67796017-ab3c-8006-96f3-186c87f0ae12

There is a certain programming principle  called "Programming to an Interface, not an Implementation".  Please scrutinize the following code making sure that it is following this principle.  Create as many new Interfaces as needed to make the code follow this principle.
```cpp
#include "SetDrawer.h"

SetDrawer::SetDrawer( RGBMatrix* canvas, GameState* gameState ) : 
    _canvas( canvas ), _gameState( gameState ), _setHistoryText( gameState ) {
    if ( _canvas == NULL ) { std::cout << "canvas is NULL" << std::endl; return; }
    FontLoader smallNumberFontLoader( LITTLE_FONT );
    rgb_matrix::Font smallNumberFont;
    smallNumberFontLoader.LoadFont( smallNumberFont );
    if ( !_little_font.LoadFont( LITTLE_FONT )) { 
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_FONT ); exit( 1 ); }}
    
SetDrawer::~SetDrawer() { 
    std::cout << "inside the SetDrawer destructor code... " << std::endl;
    std::cout << "destructing SetDrawer..." << std::endl;
}

void SetDrawer::drawTextOnCanvas( int x, int y, const Color& color, const std::string& text ) {
    print( "drawing text on canvas: " << text << " at x: " << x << " y: " << y << std::endl );
    Color background_color( 0, 0, 0 );
    int letter_spacing = 0;
    rgb_matrix::Font* outline_font = NULL;
    rgb_matrix::DrawText( _canvas, _little_font, x, y + _little_font.baseline(), color, outline_font ?
                          NULL : &background_color, text.c_str(), letter_spacing ); }

void SetDrawer::drawSets() {
    std::string playerOneSetString = _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX );
    std::string playerTwoSetString = _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX );
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        drawSetsWithSpacing( playerOneSetString, playerTwoSetString );
    }
}

void SetDrawer::blankSets() {
    std::string playerOneSetString = "";
    std::string playerTwoSetString = "";
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        drawSetsWithSpacing( playerOneSetString, playerTwoSetString );
    }
}

void SetDrawer::drawBlinkSets( int playerToBlink ) {
    print( "drawing blink sets..." );
    int set = _gameState->getCurrentSet(); // init coords and set
    std::string playerOneSetString = ""; std::string playerTwoSetString = ""; // set inside if statement
    print( "current aciton: " << _gameState->getCurrentAction());
    if ( _gameState->getCurrentAction() == BOTH_PLAYER_BLINK ) {
        playerOneSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX ), set );
        playerTwoSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX ), set );
    } else if ( playerToBlink == PLAYER_1_INITIALIZED ) { // Blink player 1
        
        playerOneSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX ), set );
        print( "player one set string after cloaker: [" << playerOneSetString << "]" );
        playerTwoSetString = _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX );
    } else {                                       // Blink player 2
        playerOneSetString = _setHistoryText.getSetHistoryText( PLAYER_ONE_SET_INDEX );
        playerTwoSetString = cloaker( _setHistoryText.getSetHistoryText( PLAYER_TWO_SET_INDEX ), set ); }
    
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    } else {
        drawSetsWithSpacing(playerOneSetString, playerTwoSetString);
    }
}

std::string SetDrawer::cloaker(std::string stringToCloak, int sectionToCloak) {
    if (sectionToCloak < 1 || sectionToCloak > 3 || stringToCloak.empty()) {
        return stringToCloak; // Return the original string if invalid input
    }

    // The pos of the digit in the string is ( 2 * section number - 2 ) ( the 1st digit is at position 0 )
    int pos = 2 * (sectionToCloak - 1); // Calculate position based on section number
    if (pos < stringToCloak.size()) {
        stringToCloak[pos] = '_'; // Use an underscore as a placeholder for blinking
    }
    
    return stringToCloak; // Return the modified string 
}


/**
 * Splits a string into a vector of substrings using the specified delimiter.
 *
 * @param str The input string to be split.
 * @param delimiter The character used to delimit the substrings.
 * @return A vector of substrings extracted from the input string.
 */
std::vector<std::string> SetDrawer::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

// Helper function to calculate the width of a string by summing character widths
int SetDrawer::getStringWidth(const std::string& text) {
    int totalWidth = 0;
    for (char c : text) {
        // Convert char to Unicode codepoint (assuming ASCII)
        uint32_t codepoint = static_cast<uint32_t>(c);
        int charWidth = _little_font.CharacterWidth(codepoint);
        if (charWidth == -1) {
            // Handle missing character by skipping or using a default width
            // Here, we'll skip adding width for missing characters
            continue;
        }
        totalWidth += charWidth;
    }
    return totalWidth;
}

// Helper function to fill a rectangle
void SetDrawer::FillRectangle(int x_start, int y_start, int width, int height, const Color& color) {
    for (int x = x_start; x < x_start + width; ++x) {
        for (int y = y_start; y < y_start + height; ++y) {
            _canvas->SetPixel(x, y, color.r, color.g, color.b);
        }
    }
}

// Helper function to draw sets for a single player
void SetDrawer::drawPlayerSets(const std::vector<std::string>& sets, Color color, int y) {
    int xStart = SMALL_BEFORE;
    int setWidth = FIXED_SET_WIDTH + SET_SPACING;
    print( "before for loop in drawPlayerSets sets.size(): " << sets.size() );
    print( "current set: " << _gameState->getCurrentSet());
    print( "sets.size(): " << sets.size() );
    // for (size_t i = 0; i < sets.size(); ++i) {
    for (size_t i = 0; i < sets.size(); ++i) {
        int x = xStart + i * setWidth;
        const std::string& set = sets[i];
        print( "drawing set: [" << set << "]" );
        int offset = (set == "1") ? OFFSET_FOR_ONE : 0;
        if ( set == "7" ) { offset = OFFSET_FOR_SEVEN; }
        drawTextOnCanvas(x + offset, y, color, set);
    }
}

// Main function to draw sets with spacing for both players
void SetDrawer::drawSetsWithSpacing(std::string playerOneSetString, std::string playerTwoSetString) {
    print( "drawing sets with spacing..." );
    print( "playerOneSetString: " + playerOneSetString );
    print( "playerTwoSetString: " + playerTwoSetString );
    int xStart = 0;      
    int yStart = START_ROW;       // Define the area to be cleared
    int width = _canvas->width();
    #define P2_Y_OFFSET -4
    int height = ( _little_font.height() + P2_Y_OFFSET ) * 2;        // Height of both rows
    FillRectangle( xStart, yStart, width, height, Color( 0, 0, 0 )); // Clear the area
    std::vector<std::string> playerOneSets = splitString( playerOneSetString ); // split into sets
    std::vector<std::string> playerTwoSets = splitString( playerTwoSetString ); 
    Color playerOneColor( 0, 255, 0 ); // Green for Player One
    Color playerTwoColor( 255, 0, 0 ); // Red for Player Two
    int yPlayerOne = START_ROW; // Define y positions for each player
    int yPlayerTwo = START_ROW + _little_font.height() + P2_Y_OFFSET; // Move to the next row
    print( "drawing player one sets..." );
    drawPlayerSets( playerOneSets, playerOneColor, yPlayerOne ); // Draw Player One's sets
    print( "drawing player two sets..." );
    drawPlayerSets( playerTwoSets, playerTwoColor, yPlayerTwo ); // Draw Player Two's sets
    print( "done drawing sets with spacing... \n\n\n" );
}
```
