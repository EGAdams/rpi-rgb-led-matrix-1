#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "../../include/graphics.h" // includes rgb_matrix::Font
#include <string>
#include <unordered_map>

class FontManager {
public:
    static const rgb_matrix::Font& getFont(const std::string& fontName);

private:
    static std::unordered_map<std::string, rgb_matrix::Font> _fontCache;
    static void _loadFont(const std::string& fontName);
};
#endif // FONTMANAGER_H
