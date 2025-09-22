/* Redirect TMPDIR so that a full /tmp doesn't cause make to crash. */
// mkdir -p ~/tmp
// export TMPDIR=~/tmp
// make

#include "SetDrawer.h"
#define P2_Y_OFFSET -4
constexpr int BOTTOM_RAISE = 3;
// ──────────────────────────────────────────────────────────────────────────────
constexpr int LEFT_BORDER_SHIFT = 4;   // move left border   ← 4 px
constexpr int RIGHT_BORDER_SHIFT = 2;   // move right border  → 2 px
// ──────────────────────────────────────────────────────────────────────────────


SetDrawer::SetDrawer( RGBMatrix* canvas, GameState* gameState ) :
    _canvas( canvas ), _gameState( gameState ), _setHistoryText( gameState ), _clockTimer( nullptr ),
    _lastBoxX( 0 ), _lastBoxY( 0 ), _lastBoxWidth( 0 ), _lastBoxHeight( 0 ) {
    if ( _canvas == NULL ) { std::cout << "canvas is NULL" << std::endl; return; }
    FontLoader smallNumberFontLoader( LITTLE_FONT );
    rgb_matrix::Font smallNumberFont;
    smallNumberFontLoader.LoadFont( smallNumberFont );
    if ( !_little_font.LoadFont( LITTLE_FONT ) ) {
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_FONT ); exit( 1 );
    }
    _clockDrawer = new ClockDrawer( _canvas );
}

SetDrawer::SetDrawer( RGBMatrix* canvas, GameState* gameState, ClockTimer* clockTimer ) :
    _canvas( canvas ), _gameState( gameState ), _setHistoryText( gameState ), _clockTimer( clockTimer ),
    _lastBoxX( 0 ), _lastBoxY( 0 ), _lastBoxWidth( 0 ), _lastBoxHeight( 0 ) {
    _clockDrawer = new ClockDrawer( _canvas );

    if ( _canvas == NULL ) { 
        std::cout << "canvas is NULL" << std::endl; 
        return;
    }

    FontLoader smallNumberFontLoader( LITTLE_FONT );
    rgb_matrix::Font smallNumberFont;
    smallNumberFontLoader.LoadFont( smallNumberFont );
    if ( !_little_font.LoadFont( LITTLE_FONT ) ) {
        fprintf( stderr, "Couldn't load font '%s'\n", LITTLE_FONT ); exit( 1 );
    }
    // std::cout << "SetDrawer created with ClockTimer: " << clockTimer << std::endl;
}

SetDrawer::~SetDrawer() {
    // std::cout << "inside the SetDrawer destructor code... " << std::endl;
    // std::cout << "destructing SetDrawer..." << std::endl;
    if ( _clockDrawer != nullptr ) {
        delete _clockDrawer;
        _clockDrawer = nullptr;
    }
}

void SetDrawer::drawTextOnCanvas( int x,
                                 int y,
                                 const Color& color,
                                 const std::string& text )
{
    // ──────────────────────────────────────────────────────────────────────────
    //  Diagnostics
    // ──────────────────────────────────────────────────────────────────────────
    // std::cout << "[DEBUG] *** DRAWING TEXT: '" << text << "' at (" << x << ", " << y << ") ***" << std::endl;
    int letter_spacing = 0;
    /* bounding box of the string we’re about to draw */
    int baseline_y = y + _little_font.baseline();
    int text_width = _getStringWidth( text );
    int text_bottom = baseline_y + _little_font.height();

    // std::cout << "[DEBUG] Text area: x=" << x << "–" << (x + text_width)
    //           << ", y=" << y << "–" << text_bottom
    //           << " (baseline_y=" << baseline_y
    //           << ", font_height=" << _little_font.height() << ')' << std::endl;

    // ──────────────────────────────────────────────────────────────────────────
    //  Check overlap with set-box so we know whether to re-draw divider
    // ──────────────────────────────────────────────────────────────────────────
    bool overlaps_set_box = false;
    if ( _lastBoxWidth > 0 && _lastBoxHeight > 0 ) {
        bool x_overlap = ( x < _lastBoxX + _lastBoxWidth ) &&
            ( x + text_width > _lastBoxX );
        bool y_overlap = ( y < _lastBoxY + _lastBoxHeight ) &&
            ( text_bottom > _lastBoxY );
        overlaps_set_box = x_overlap && y_overlap;

        if ( overlaps_set_box )
        {
            // std::cout << "[WARNING] *** TEXT OVERLAPS SET BOX AREA! ***" << std::endl;
        }
    }

    // ──────────────────────────────────────────────────────────────────────────
    //  Draw the text (no background color so we don’t wipe pixels)
    // ──────────────────────────────────────────────────────────────────────────
    rgb_matrix::DrawText( _canvas, _little_font, x, baseline_y,
                         color, nullptr, text.c_str(), letter_spacing );

    // std::cout << "[DEBUG] *** TEXT DRAWING COMPLETE ***" << std::endl;

    // ──────────────────────────────────────────────────────────────────────────
    //  EMERGENCY FIX – re-draw centre divider *inside* the frame only
    // ──────────────────────────────────────────────────────────────────────────
    if ( overlaps_set_box )
    {
        constexpr int BOTTOM_RAISE = 3;              // keep in sync everywhere
        const Color blue( 0, 0, 255 );

        const int x_left = _lastBoxX + ( _lastBoxWidth / 2 ); // column 1

        const int y_top = _lastBoxY;                                 // top row
        const int y_border = _lastBoxY + _lastBoxHeight - 1 - BOTTOM_RAISE; // bottom edge

        /* a) erase any old tail pixels beneath the frame */
        for ( int yy = y_border + 1; yy < _lastBoxY + _lastBoxHeight; ++yy )
        {
            _canvas->SetPixel( x_left, yy, 0, 0, 0 );
            // _canvas->SetPixel(x_right, yy, 0, 0, 0);
        }

        /* b) redraw the 1-pixel-wide divider inside the frame */
        for ( int yy = y_top; yy <= y_border; ++yy )
        {
            _canvas->SetPixel( x_left, yy, blue.r, blue.g, blue.b );
            // _canvas->SetPixel(x_right, yy, blue.r, blue.g, blue.b);
        }

        // std::cout << "[EMERGENCY FIX] Centre divider restored - clipped at " << "y=" << y_border << std::endl;
    }
}

int SetDrawer::getSetFontHeight() const {
    return _little_font.height();
}

// ──────────────────────────────────────────────────────────────────────────────
// 1. Public entry-point used by the main drawing loop
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::drawSets()
{
    if ( _canvas == nullptr )  {
        _drawPickleSets();
        if ( _clockDrawer != nullptr ) {
        _clockDrawer->drawClock( _lastBoxX, _lastBoxY, _lastBoxWidth, _lastBoxHeight, _clockTimer );
    }
        return;
    }

    // ① First calculate box dimensions but don't draw yet
    // std::cout << "calculateBoxDimensions() <enter>" << std::endl;
    _calculateBoxDimensions();

    // ② Draw sets first (so they don't overwrite diagonal corners)
    // std::cout << "drawPickleSets() <enter>" << std::endl;
    _drawPickleSets();     // Draw digits first

    // ③ Now draw the box borders with diagonal corners OVER the sets
    // std::cout << "drawBoxBorders() <enter>" << std::endl;
    _drawBoxBorders();     // Draw borders with diagonal corners

    // ④ Draw center divider last to ensure it's never overwritten
    // std::cout << "drawCenterDivider() <enter>" << std::endl;
    drawCenterDivider();

    // ⑤ Draw clock under the Set Blue Box
    // std::cout << "drawClock() <enter>" << std::endl;
    if ( _clockDrawer != nullptr ) {
        _clockDrawer->drawClock( _lastBoxX, _lastBoxY, _lastBoxWidth, _lastBoxHeight, _clockTimer );
    }

    // ⑥ VERIFICATION: Check if center divider pixels are actually set
    // std::cout << "[VERIFICATION] Checking if center divider pixels are actually blue..." << std::endl;
    const int middle_x = _lastBoxX + ( _lastBoxWidth / 2 );
    const int test_y = _lastBoxY + 2;  // Test a pixel near the top where the gap appears

    // Note: We can't directly read pixel values from the canvas, but we can verify our coordinates
    // std::cout << "[VERIFICATION] Center divider should be at x=" << middle_x << ", testing at y=" << test_y << std::endl;
    // std::cout << "[VERIFICATION] Diagonal corners should now be preserved!" << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
// 1. 2-pixel-wide divider that spans the *entire* height of the set box
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::drawCenterDivider() {
    // std::cout << "[DEBUG] drawCenterDivider() START - using detailed implementation" << std::endl;
    _drawCenterDividerDetailed();
    // std::cout << "[DEBUG] drawCenterDivider() END" << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
//  2-pixel-wide centre divider – clipped so NOTHING shows below the frame
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::_drawCenterDividerDetailed()
{
    if ( _canvas == nullptr || _lastBoxWidth == 0 || _lastBoxHeight == 0 )
        return;

    constexpr int BOTTOM_RAISE = 3;                          // keep in sync // also defined in .h file
    const Color blue( 0, 0, 255 );

    /* divider column */
    const int x1 = _lastBoxX + ( _lastBoxWidth / 2 );      // left column

    /* vertical limits */
    const int y_top = _lastBoxY;                        // top border row
    const int y_border = _lastBoxY + _lastBoxHeight - 1 -
        BOTTOM_RAISE;                     // row of bottom border
    const int y_old_tail = _lastBoxY + _lastBoxHeight - 1;   // where the tail used to end

    // ① ERASE any stray pixels *below* the border on BOTH columns
    for ( int y = y_border + 1; y <= y_old_tail; ++y )
    {
        _canvas->SetPixel( x1, y, 0, 0, 0 );
        // _canvas->SetPixel(x2, y, 0, 0, 0);
    }

    // ② REDRAW clean divider inside the frame (stop right at the border)
    for ( int y = y_top; y <= y_border; ++y )
    {
        _canvas->SetPixel( x1, y, blue.r, blue.g, blue.b );
        // _canvas->SetPixel(x2, y, blue.r, blue.g, blue.b);
    }
}



void SetDrawer::drawSetBox() {
    // std::cout << "[DEBUG] ===== drawSetBox() START =====" << std::endl;

    if ( _canvas == nullptr ) {
        std::cout << "[ERROR] drawSetBox: canvas is nullptr!" << std::endl;
        return;
    }

    // std::cout << "[DEBUG] Step 1: Calculate and store box dimensions" << std::endl;
    _calculateBoxDimensions();

    // std::cout << "[DEBUG] Step 2: Draw the four borders" << std::endl;
    _drawBoxBorders();

    // std::cout << "[DEBUG] Step 3: Draw the center divider" << std::endl;
    drawCenterDivider();

    // std::cout << "[DEBUG] ===== drawSetBox() END =====" << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
//  Draw the green / red set digits inside the blue-frame box
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::_drawPickleSets() {
    // std::cout << "[DEBUG] ===== _drawPickleSets() START =====" << std::endl;
    
    const std::string left_sets = std::to_string( _gameState->getTeamASets() );
    const std::string right_sets = std::to_string( _gameState->getTeamBSets() );

    if ( _canvas == nullptr ) {
        std::cout << "Set scores: GREEN: '" << left_sets << "', RED: '" << right_sets << "'" << std::endl;
        return;
    }

    // ──────────────────────────────────────────────────────────────────────
    //  1. Clear the interior of each half (leave chamfer-corners intact)
    // ──────────────────────────────────────────────────────────────────────
    const int corner_size = 3;
    const int divider_width = 2;

    const int middle_x = _lastBoxX + ( _lastBoxWidth / 2 );   // left column of divider
    // std::cout << "[DEBUG] Calculated middle_x for clearing: " << middle_x << std::endl;

    const int inner_y = _lastBoxY + corner_size + 1;
    const int inner_h = _lastBoxHeight - ( 2 * corner_size + 2 );

    // ---- left half ------------------------------------------------------
    const int left_inner_x = _lastBoxX + corner_size + 1;
    const int left_inner_w = middle_x - left_inner_x;                 // up to divider
    // std::cout << "[DEBUG] LEFT clear area ( avoiding corners ): x=" << left_inner_x << ", y=" << inner_y << ", w=" << left_inner_w << ", h=" << inner_h << std::endl;
    if ( left_inner_w > 0 && inner_h > 0 ) {
        _fillRectangle( left_inner_x, inner_y, left_inner_w, inner_h, Color( 0, 0, 0 ) );
    }
    else {
        std::cout << "[WARNING] LEFT clear dimensions invalid: w=" << left_inner_w << ", h=" << inner_h << std::endl;
    }

    // ---- right half -----------------------------------------------------
    const int right_inner_x = middle_x + divider_width;                // start past divider
    const int right_inner_w = ( _lastBoxX + _lastBoxWidth - corner_size - 1 ) - right_inner_x;
    // std::cout << "[DEBUG] RIGHT clear area ( avoiding corners ): x=" << right_inner_x << ", y=" << inner_y << ", w=" << right_inner_w << ", h=" << inner_h << std::endl;

    if ( right_inner_w > 0 && inner_h > 0 ) {
        _fillRectangle( right_inner_x, inner_y, right_inner_w, inner_h, Color( 0, 0, 0 ) );
    }
    else {
        // std::cout << "[WARNING] RIGHT clear dimensions invalid: w=" << right_inner_w << ", h=" << inner_h << std::endl;
    }

    // ──────────────────────────────────────────────────────────────────────
    //  2. Gather data & pick colours
    // ──────────────────────────────────────────────────────────────────────
    // std::cout << "[DEBUG] Set scores: left='" << left_sets << "', right='" << right_sets << "'" << std::endl;
    const Color green( 0, 255, 0 );
    const Color red( 255, 0, 0 );

    // ──────────────────────────────────────────────────────────────────────
    //  3. Horizontal centring ( share spare pixel with right-hand half )
    // ──────────────────────────────────────────────────────────────────────
    const int half_w = _lastBoxWidth / 2;

    auto centredX = [&]( const std::string& txt, bool isRight ) -> int
        {
            int txt_w = _getStringWidth( txt );
            int free_px = half_w - txt_w;
            int offset = free_px / 2;                      // floor division
            if ( isRight && ( free_px % 2 ) != 0 )          // give extra px to right
            {
                ++offset;
            }
            return _lastBoxX + ( isRight ? half_w : 0 ) + offset;
        };

    const int left_x = centredX( left_sets, false ) + ( ( left_sets == "1" ) ? 3 : 2 );
    const int right_x = centredX( right_sets, true ) + ( ( right_sets == "1" ) ? 2 : 1 );

    // std::cout << "[DEBUG] Text positioning: left_x=" << left_x << ", right_x=" << right_x << std::endl;
    // ──────────────────────────────────────────────────────────────────────
    //  4. Vertical centring
    // ──────────────────────────────────────────────────────────────────────
    const int v_pad = ( inner_h - _little_font.height() ) / 2;
    const int top_y = _lastBoxY + corner_size + 1 + v_pad;
    // std::cout << "[DEBUG] Vertical positioning: top_y=" << top_y << ", v_pad=" << v_pad << std::endl;
    // ──────────────────────────────────────────────────────────────────────
    //  5. Render digits
    // ──────────────────────────────────────────────────────────────────────
    // std::cout << "[DEBUG] Rendering left set text: " << left_sets << "..." << std::endl;
    drawTextOnCanvas( left_x, top_y, green, left_sets );

    // std::cout << "[DEBUG] Rendering right set text: " << right_sets << "..." << std::endl;
    drawTextOnCanvas( right_x, top_y, red, right_sets );

    // std::cout << "[DEBUG] Sets drawn – borders will be drawn next to preserve diagonal corners" << std::endl;
    // std::cout << "[DEBUG] ===== _drawPickleSets() END =====" << std::endl;
}

void SetDrawer::blankSets() {
    std::string playerOneSetString = "";
    std::string playerTwoSetString = "";
    if ( _canvas == NULL ) {
        std::cout << playerOneSetString << std::endl;
        std::cout << playerTwoSetString << std::endl;
    }
    else {
        _drawSetsWithSpacing( playerOneSetString, playerTwoSetString );
    }
}

void SetDrawer::blinkPickleSets( int winning_team_number ) {
    constexpr int BLINK_COUNT = 5;
    constexpr int BLINK_INTERVAL_MS = 1000;

    for ( int i = 0; i < BLINK_COUNT; ++i ) {
        // Step 1: Draw the complete set box but with the winning set score cleared
        {
            // Calculate box dimensions first
            _calculateBoxDimensions();

            // Draw the complete set box with borders, center divider, and set scores
            drawSets();

            // Now selectively clear only the winning team's set score
            _clearWinningSetScore( winning_team_number );
        }
        GameTimer::gameDelay( BLINK_INTERVAL_MS );

        // Step 2: Draw the complete set box (turn on everything including winning score)
        drawSets();
        GameTimer::gameDelay( BLINK_INTERVAL_MS );
    }
}
std::string SetDrawer::cloaker( std::string stringToCloak, int sectionToCloak ) {
    if ( sectionToCloak < 1 || sectionToCloak > 3 ) { return "Invalid section number"; }
    int pos = 2 * ( sectionToCloak - 1 ); // The pos of the digit in the string is ( 2 * section number - 2 ) ( the 1st digit is at position 0 )
    stringToCloak[pos] = ' ';           // Replace the character at the calculated position with a space
    return stringToCloak;
}

// set drawer additions...
/**
 * Splits a string into a vector of substrings using the specified delimiter.
 *
 * @param str The input string to be split.
 * @param delimiter The character used to delimit the substrings.
 * @return A vector of substrings extracted from the input string.
 */
std::vector<std::string> SetDrawer::splitString( const std::string& str, char delimiter ) {
    std::vector<std::string> tokens;
    std::stringstream ss( str );
    std::string token;

    while ( std::getline( ss, token, delimiter ) ) {
        if ( !token.empty() ) {
            tokens.push_back( token );
        }
    }

    return tokens;
}

// Helper function to calculate the width of a string by summing character widths
int SetDrawer::_getStringWidth( const std::string& text ) {
    int totalWidth = 0;
    for ( char c : text ) {
        // Convert char to Unicode codepoint (assuming ASCII)
        uint32_t codepoint = static_cast< uint32_t >( c );
        int charWidth = _little_font.CharacterWidth( codepoint );
        if ( charWidth == -1 ) {
            // Handle missing character by skipping or using a default width
            // Here, we'll skip adding width for missing characters
            continue;
        }
        totalWidth += charWidth;
    }
    return totalWidth;
}

// Helper function to fill a rectangle
void SetDrawer::_fillRectangle( int x_start, int y_start, int width, int height, const Color& color ) {
    for ( int x = x_start; x < x_start + width; ++x ) {
        for ( int y = y_start; y < y_start + height; ++y ) {
            _canvas->SetPixel( x, y, color.r, color.g, color.b );
        }
    }
}

// Helper function to draw sets for a single player
void SetDrawer::_drawPlayerSets( const std::vector<std::string>& sets, Color color, int y ) {
    int xStart = SMALL_BEFORE;
    int setWidth = FIXED_SET_WIDTH + SET_SPACING;
    // print( "before for loop in drawPlayerSets sets.size(): " << sets.size() );
    // print( "current set: " << _gameState->getCurrentSet() );
    // print( "sets.size(): " << sets.size() );
    for ( size_t i = 0; i < sets.size(); ++i ) {
        int x = xStart + i * setWidth;
        const std::string& set = sets[i];
        // print( "drawing set: [" << set << "]" );
        int offset = ( set == "1" ) ? OFFSET_FOR_ONE : 0;
        if ( set == "7" ) { offset = OFFSET_FOR_SEVEN; }
        drawTextOnCanvas( x + offset, y, color, set );
    }
}

// Main function to draw sets with spacing for both players
void SetDrawer::_drawSetsWithSpacing( std::string playerOneSetString, std::string playerTwoSetString ) {
    // print( "drawing sets with spacing..." );
    // print( "playerOneSetString: " + playerOneSetString );
    // print( "playerTwoSetString: " + playerTwoSetString );
    int xStart = 0;
    int yStart = START_ROW;       // Define the area to be cleared
    int width = _canvas->width();
    int height = ( _little_font.height() + P2_Y_OFFSET ) * 2;        // Height of both rows
    _fillRectangle( xStart, yStart, width, height, Color( 0, 0, 0 ) ); // Clear the area
    std::vector<std::string> playerOneSets = splitString( playerOneSetString ); // split into sets
    std::vector<std::string> playerTwoSets = splitString( playerTwoSetString );
    Color playerOneColor( 0, 255, 0 ); // Green for Player One
    Color playerTwoColor( 255, 0, 0 ); // Red for Player Two
    int yPlayerOne = START_ROW; // Define y positions for each player
    int yPlayerTwo = START_ROW + _little_font.height() + P2_Y_OFFSET; // Move to the next row
    // print( "drawing player one sets..." );
    _drawPlayerSets( playerOneSets, playerOneColor, yPlayerOne ); // Draw Player One's sets
    // print( "drawing player two sets..." );
    _drawPlayerSets( playerTwoSets, playerTwoColor, yPlayerTwo ); // Draw Player Two's sets
    // print( "done drawing sets with spacing... \n\n\n" );
}

// ──────────────────────────────────────────────────────────────────────────────
//  Calculate and store box dimensions for the blue-set box
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::_calculateBoxDimensions()
{
    // std::cout << "[DEBUG] _calculateBoxDimensions() START" << std::endl;

    if ( _canvas == nullptr )
    {
        std::cout << "[ERROR] _calculateBoxDimensions: canvas is nullptr!" << std::endl;
        return;
    }

    // ────────────────────────────────
    //  Basic geometry
    // ────────────────────────────────
    int  box_padding = 2;
    int  box_width = 33;
    int  box_height = _little_font.height() + ( box_padding * 2 ) - 2;  // raise bottom edge

    // std::cout << "[DEBUG] Font height:          " << _little_font.height() << std::endl;
    // std::cout << "[DEBUG] Box padding:          " << box_padding          << std::endl;
    // std::cout << "[DEBUG] Calculated box_height:" << box_height           << std::endl;

    int  canvas_width = _canvas->width();
    int  canvas_height = _canvas->height();
    // std::cout << "[DEBUG] Canvas dimensions:    " << canvas_width << "x" << canvas_height << std::endl;

    int  box_x = ( canvas_width - box_width ) / 2 - 1 + 2;    // historical tweaks remain

    // 2-pixel gap above current-game zeros
    int  desired_gap = 2;
    int  adjustment_up = 20 + 4;                               // empirical offset
    int  zero_bottom = START_ROW + _little_font.height();
    int  box_y = zero_bottom + desired_gap - adjustment_up;

    // std::cout << "[DEBUG] START_ROW:            " << START_ROW            << std::endl;
    // std::cout << "[DEBUG] Zero bottom:          " << zero_bottom          << std::endl;
    // std::cout << "[DEBUG] Desired gap:          " << desired_gap          << std::endl;
    // std::cout << "[DEBUG] Adjustment up:        " << adjustment_up        << std::endl;
    // std::cout << "[DEBUG] box_y calculated:     " << box_y                << std::endl;

    if ( box_y < 0 )
    {
        // std::cout << "[DEBUG] Clamping box_y from " << box_y << " to 0" << std::endl;
        box_y = 0;                                             // keep on-screen
    }

    // ────────────────────────────────
    //  NEW – shift borders only
    // ────────────────────────────────
    constexpr int LEFT_BORDER_SHIFT = 4;                      // move left edge ← 4 px
    constexpr int RIGHT_BORDER_SHIFT = 2;                      // move right edge → 2 px

    box_x -= LEFT_BORDER_SHIFT;                              // pull frame left
    box_width += LEFT_BORDER_SHIFT + RIGHT_BORDER_SHIFT;       // widen span

    // ────────────────────────────────
    //  Persist for other routines
    // ────────────────────────────────
    _lastBoxX = box_x;
    _lastBoxY = box_y;
    _lastBoxWidth = box_width;
    _lastBoxHeight = box_height;

    // std::cout << "[FINAL] Box dimensions stored:" << std::endl;
    // std::cout << "  _lastBoxX      = " << _lastBoxX << std::endl;
    // std::cout << "  _lastBoxY      = " << _lastBoxY << std::endl;
    // std::cout << "  _lastBoxWidth  = " << _lastBoxWidth << std::endl;
    // std::cout << "  _lastBoxHeight = " << _lastBoxHeight << std::endl;
    // std::cout << "[DEBUG] _calculateBoxDimensions() END"      << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
// Draw the four borders of the blue set box
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::_drawBoxBorders() {
    // std::cout << "[DEBUG] _drawBoxBorders() START" << std::endl;

    if ( _canvas == nullptr ) {
        std::cout << "[ERROR] _drawBoxBorders: canvas is nullptr!" << std::endl;
        return;
    }

    // std::cout << "[DEBUG] Drawing borders using stored dimensions:" << std::endl;
    // std::cout << "  Box: (" << _lastBoxX << ", " << _lastBoxY << ") "
    //     << _lastBoxWidth << "x" << _lastBoxHeight << std::endl;

    _drawLeftBorder();
    _drawRightBorder();
    _drawTopBorder();
    _drawBottomBorder();

    // std::cout << "[DEBUG] _drawBoxBorders() END" << std::endl;
}

// ──────────────────────────────────────────────────────────────────────────────
// Draw individual border components with detailed logging
// ──────────────────────────────────────────────────────────────────────────────
void SetDrawer::_drawLeftBorder() {
    Color blue( 0, 0, 255 );
    const int corner_size = 3;
    const int bottom_y = _lastBoxY + _lastBoxHeight - 1 - BOTTOM_RAISE;

    for ( int y = _lastBoxY; y <= bottom_y; ++y )
    {
        // skip three-pixel chamfers top **and bottom**
        if ( y < _lastBoxY + corner_size ||
            y > bottom_y - corner_size )               // NEW check
            continue;

        _canvas->SetPixel( _lastBoxX, y, blue.r, blue.g, blue.b );
    }
}


void SetDrawer::_drawRightBorder()
{
    Color blue( 0, 0, 255 );
    const int corner_size = 3;
    const int right_x = _lastBoxX + _lastBoxWidth - 1;
    const int bottom_y = _lastBoxY + _lastBoxHeight - 1 - BOTTOM_RAISE;

    for ( int y = _lastBoxY; y <= bottom_y; ++y )
    {
        if ( y < _lastBoxY + corner_size ||
            y > bottom_y - corner_size )               // NEW check
            continue;

        _canvas->SetPixel( right_x, y, blue.r, blue.g, blue.b );
    }
}

void SetDrawer::_drawTopBorder() {
    // std::cout << "[DEBUG] Drawing TOP border with stop-sign style corners from x=" << _lastBoxX 
    //           << " to x=" << (_lastBoxX + _lastBoxWidth - 1) 
    //           << " at y=" << _lastBoxY << std::endl;

    Color blueColor( 0, 0, 255 );
    const int center_x1 = _lastBoxX + ( _lastBoxWidth / 2 );
    const int center_x2 = center_x1 + 1;
    const int corner_size = 3; // Clip 3 pixels from each corner for stop-sign effect

    for ( int x = _lastBoxX; x < _lastBoxX + _lastBoxWidth; x++ ) {
        // Skip corner pixels to create pronounced diagonal effect
        if ( x < _lastBoxX + corner_size || x >= _lastBoxX + _lastBoxWidth - corner_size ) {
            continue; // Skip corner areas
        }

        _canvas->SetPixel( x, _lastBoxY, blueColor.r, blueColor.g, blueColor.b );

        // CRITICAL CHECK: Are we overwriting center divider pixels?
        if ( x == center_x1 || x == center_x2 ) {
            // std::cout << "[CRITICAL] TOP border drawing over center divider at x=" << x
            //     << ", y=" << _lastBoxY << " - this SHOULD be fine since they're both blue!" << std::endl;
        }
    }

    // Draw diagonal lines for top corners (stop-sign style)
    for ( int i = 0; i < corner_size; i++ ) {
        // Top-left diagonal
        _canvas->SetPixel( _lastBoxX + corner_size - 1 - i, _lastBoxY + i + 1, blueColor.r, blueColor.g, blueColor.b );
        // Top-right diagonal  
        _canvas->SetPixel( _lastBoxX + _lastBoxWidth - corner_size + i, _lastBoxY + i + 1, blueColor.r, blueColor.g, blueColor.b );
    }
}

void SetDrawer::_drawBottomBorder() {
    const int corner_size = 3;
    const int bottom_y =
        _lastBoxY + _lastBoxHeight - 1 - BOTTOM_RAISE;   // one clean row

    Color blue( 0, 0, 255 );

    /* straight segment */
    for ( int x = _lastBoxX; x < _lastBoxX + _lastBoxWidth; ++x )
    {
        if ( x < _lastBoxX + corner_size ||
            x >= _lastBoxX + _lastBoxWidth - corner_size )
            continue;

        _canvas->SetPixel( x, bottom_y, blue.r, blue.g, blue.b );
    }

    /* 45° chamfers (mirror of the top) */
    for ( int i = 0; i < corner_size; ++i )
    {
        _canvas->SetPixel( _lastBoxX + corner_size - 1 - i,
                          bottom_y - i - 1, blue.r, blue.g, blue.b );
        _canvas->SetPixel( _lastBoxX + _lastBoxWidth - corner_size + i,
                          bottom_y - i - 1, blue.r, blue.g, blue.b );
    }
}

// Helper method to clear only the winning team's set score
void SetDrawer::_clearWinningSetScore( int winning_team_number ) {
    if ( _canvas == nullptr ) return;

    // Get the set scores to determine positioning
    const std::string left_sets = std::to_string( _gameState->getTeamASets() );
    const std::string right_sets = std::to_string( _gameState->getTeamBSets() );

    // Calculate dimensions and positioning (same logic as in _drawPickleSets)
    const int corner_size = 3;
    const int inner_h = _lastBoxHeight - ( 2 * corner_size + 2 );
    const int half_w = _lastBoxWidth / 2;

    // Horizontal centering function (same as in _drawPickleSets)
    auto centredX = [&]( const std::string& txt, bool isRight ) -> int {
        int txt_w = _getStringWidth( txt );
        int free_px = half_w - txt_w;
        int offset = free_px / 2;
        if ( isRight && ( free_px % 2 ) != 0 ) {
            ++offset;
        }
        return _lastBoxX + ( isRight ? half_w : 0 ) + offset;
        };

    // Vertical centering
    const int v_pad = ( inner_h - _little_font.height() ) / 2;
    const int top_y = _lastBoxY + corner_size + 1 + v_pad;

    // Clear the appropriate set score based on winning team
    if ( winning_team_number == 1 ) {
        // Team A (left side) won - clear left set score
        const int left_x = centredX( left_sets, false ) + ( ( left_sets == "1" ) ? 3 : 2 );
        const int text_width = _getStringWidth( left_sets );
        const int text_height = _little_font.height();

        _fillRectangle( left_x, top_y, text_width, text_height, Color( 0, 0, 0 ) );
    }
    else if ( winning_team_number == 2 ) {
        // Team B (right side) won - clear right set score
        const int right_x = centredX( right_sets, true ) + ( ( right_sets == "1" ) ? 2 : 1 );
        const int text_width = _getStringWidth( right_sets );
        const int text_height = _little_font.height();

        _fillRectangle( right_x, top_y, text_width, text_height, Color( 0, 0, 0 ) );
    }

    // Redraw the bottom border and center divider in case the text clearing affected them
    _drawBottomBorder();
    drawCenterDivider();
}

void SetDrawer::setClockTimer(ClockTimer* clockTimer) {
    _clockTimer = clockTimer;
    std::cout << "ClockTimer set in SetDrawer: " << clockTimer << std::endl;
}

