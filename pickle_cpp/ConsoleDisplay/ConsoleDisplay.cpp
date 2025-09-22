#include "../ColorManager/ColorManager.h"
#include "../ConsoleDisplay/ConsoleDisplay.h"

ConsoleDisplay::ConsoleDisplay( ColorManager* colorManager ) : _colorManager( colorManager ) {
    _current_color_string = "WHITE";
}

void ConsoleDisplay::setColor( const std::string& color ) { 
    _current_color_string = color; 
}

void ConsoleDisplay::drawText( const std::string& text, int x, int y ) {
    const std::string& colorCode = ColorManager::getAsciiColorCode( _current_color_string );
    const std::string& resetCode = ColorManager::getAsciiColorCode( "RESET" );

    // Move the cursor to the specified position
    std::cout << "\033[" << y << ";" << x << "H";  // Cursor movement to (x, y)

    // Output the text with the color and then reset the color
    std::cout << colorCode << text << resetCode << std::endl;
}

void ConsoleDisplay::clearScreen() { std::cout << "\033[2J\033[H";}
