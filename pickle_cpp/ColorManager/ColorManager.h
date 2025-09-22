#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include "../../include/graphics.h" // rgb_matrix::Color defined here
#include <string>
#include <unordered_map>

class ColorManager {
public:
    static const rgb_matrix::Color& getColor(const std::string& colorName);
    static const std::string& getAsciiColorCode(const std::string& colorName);
};
#endif // COLORMANAGER_H