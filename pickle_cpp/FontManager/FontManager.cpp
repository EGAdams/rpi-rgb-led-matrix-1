#include "FontManager.h"
#include <iostream>

std::unordered_map<std::string, rgb_matrix::Font> FontManager::_fontCache;

const rgb_matrix::Font& FontManager::getFont( const std::string& fontName ) {
    auto it = _fontCache.find( fontName ); // Search for the font in the cache
    if ( it != _fontCache.end() ) {
        std::cout << "Font '" << fontName << "' found in cache." << std::endl;
        return it->second;
    }

    std::cout << "Font '" << fontName << "' not found in cache. Loading font." << std::endl;
    _loadFont( fontName );

    it = _fontCache.find( fontName ); // After loading, ensure it has been successfully cached
    if ( it == _fontCache.end() ) {
        std::cerr << "Error: Font '" << fontName << "' could not be loaded into the cache." << std::endl;
        exit( 1 );
    }
    return it->second;
}

void FontManager::_loadFont( const std::string& fontName ) {
    rgb_matrix::Font font;

    if ( !font.LoadFont( fontName.c_str())) { // Attempt to load the font file
        std::cerr << "Failed to load font: " << fontName << std::endl;
        exit( 1 );
    }

    _fontCache[ fontName ] = font; // Store the font in the cache
    std::cout << "Font '" << fontName << "' loaded and cached." << std::endl;
}
