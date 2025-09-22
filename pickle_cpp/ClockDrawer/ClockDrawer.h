#ifndef CLOCK_DRAWER_H
#define CLOCK_DRAWER_H

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "../../include/led-matrix.h"
#include "../../include/graphics.h"
#include "../FontLoader/FontLoader.h"
#include "../ClockTimer/ClockTimer.h"

using namespace rgb_matrix;

#define CLOCK_FONT        "../fonts/6x12.bdf"
#define CLOCK_FONT_OFFSET 2

class ClockDrawer {
public:
    ClockDrawer(RGBMatrix* canvas);
    ~ClockDrawer();
    
    void drawClock(int box_x, int box_y, int box_width, int box_height, ClockTimer* clockTimer = nullptr);
    int getClockFontHeight() const;

private:
    RGBMatrix* _canvas;
    rgb_matrix::Font _clock_font;
    
    std::string _getCurrentTimeString();
    std::string _getElapsedTimeString(ClockTimer* clockTimer);
    int _getStringWidth(const std::string& text);
};

#endif