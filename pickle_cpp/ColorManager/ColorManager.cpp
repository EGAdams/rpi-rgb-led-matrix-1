#include "ColorManager.h"

const rgb_matrix::Color& ColorManager::getColor( const std::string& colorName ) {
    static const std::unordered_map<std::string, rgb_matrix::Color> colorMap = {
        {"RED", rgb_matrix::Color( 255, 0, 0 )},
        {"GREEN", rgb_matrix::Color( 0, 255, 0 )},
        {"BLUE", rgb_matrix::Color( 0, 0, 255 )},
        {"YELLOW", rgb_matrix::Color( 255, 255, 0 )},
        {"ORANGE", rgb_matrix::Color( 255, 165, 0 )},
        {"PURPLE", rgb_matrix::Color( 128, 0, 128 )},
        {"WHITE", rgb_matrix::Color( 255, 255, 255 )},
        {"BLACK", rgb_matrix::Color( 0, 0, 0 )}
    };

    auto it = colorMap.find( colorName );
    if ( it != colorMap.end() ) {
        return it->second;
    }
    else {
        static const rgb_matrix::Color black( 0, 0, 0 );
        return black;
    }
}

const std::string& ColorManager::getAsciiColorCode( const std::string& colorName ) {
    static const std::unordered_map<std::string, std::string> asciiColorMap = {
        {"RED", "\033[31m"},
        {"GREEN", "\033[32m"},
        {"BLUE", "\033[34m"},
        {"YELLOW", "\033[33m"},
        {"ORANGE", "\033[38;5;214m"},
        {"PURPLE", "\033[35m"},
        {"WHITE", "\033[37m"},
        {"BLACK", "\033[30m"},
        {"RESET", "\033[0m"}
    };

    auto it = asciiColorMap.find( colorName );
    if ( it != asciiColorMap.end() ) {
        return it->second;
    }
    else {
        static const std::string reset = "\033[0m";
        return reset;
    }
}
