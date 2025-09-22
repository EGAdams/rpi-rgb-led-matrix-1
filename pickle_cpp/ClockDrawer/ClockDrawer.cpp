#include "ClockDrawer.h"

ClockDrawer::ClockDrawer( RGBMatrix* canvas ) : _canvas( canvas ) {
    if ( _canvas == nullptr ) {
        std::cout << "ClockDrawer: canvas is nullptr, " << std::endl;
        return;
    }

    if ( !_clock_font.LoadFont( CLOCK_FONT )) {
        fprintf( stderr, "ClockDrawer: Couldn't load font '%s'\n", CLOCK_FONT );
        exit( 1 );
    }
}

ClockDrawer::~ClockDrawer() {
    std::cout << "ClockDrawer destructor called" << std::endl;
}

void ClockDrawer::drawClock( int box_x, int box_y, int box_width, int box_height, ClockTimer* clockTimer ) {

    std::string timeStr;
    if ( clockTimer != nullptr ) {
        timeStr = _getElapsedTimeString( clockTimer );
    }
    else {
        timeStr = _getCurrentTimeString();
    }

    if ( _canvas == nullptr ) {
        std::cout << timeStr.c_str() << std::endl;
        return;
    }

    int clock_x = box_x + ( box_width / 2 ) - ( _getStringWidth( timeStr ) / 2 );
    int clock_y = box_y + box_height + 5;
    Color yellow_color( 255, 255, 0 );
    int baseline_y = clock_y + _clock_font.baseline() - CLOCK_FONT_OFFSET;
    rgb_matrix::DrawText( _canvas, _clock_font, clock_x, baseline_y,
                        yellow_color, nullptr, timeStr.c_str(), 0 );
}

int ClockDrawer::getClockFontHeight() const {
    return _clock_font.height();
}

std::string ClockDrawer::_getCurrentTimeString() {
    std::time_t now = std::time( nullptr );
    std::tm* localTime = std::localtime( &now );

    std::ostringstream timeStream;
    timeStream << std::setfill( '0' ) << std::setw( 2 ) << localTime->tm_hour
        << ":" << std::setfill( '0' ) << std::setw( 2 ) << localTime->tm_min;

    return timeStream.str();
}

std::string ClockDrawer::_getElapsedTimeString( ClockTimer* clockTimer ) {
    if ( clockTimer == nullptr ) {
        return "00:00";
    }

    int totalSeconds = clockTimer->getElapsedSeconds();
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream timeStream;
    timeStream << std::setfill( '0' ) << std::setw( 2 ) << minutes
        << ":" << std::setfill( '0' ) << std::setw( 2 ) << seconds;

    return timeStream.str();
}

int ClockDrawer::_getStringWidth( const std::string& text ) {
    int totalWidth = 0;
    for ( char c : text ) {
        uint32_t codepoint = static_cast< uint32_t >( c );
        int charWidth = _clock_font.CharacterWidth( codepoint );
        if ( charWidth == -1 ) {
            continue;
        }
        totalWidth += charWidth;
    }
    return totalWidth;
}
