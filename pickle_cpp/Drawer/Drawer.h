#ifndef NUMBER_DRAWER_H
#define NUMBER_DRAWER_H

#include "../../include/led-matrix.h"
#include "../../include/graphics.h"
#include <iostream>

using namespace rgb_matrix;

class Drawer {
public:
    enum Size { BIG, SMALL };

    Drawer( RGBMatrix* canvas, const rgb_matrix::Font* font, Size size, const Color& color, const Color& bg_color );
    void drawNumber( const std::string& number, int x, int y );
    void drawText(   const std::string& text,   int x, int y );
    void setForegroundColor( const Color& color );
    void setBackgroundColor( const Color& color );
    void setFont( const rgb_matrix::Font* font  );
    void setSize( Size                    size  );

private:
    RGBMatrix*              _canvas;
    const rgb_matrix::Font* _font;
    Size                    _size;
    Color                   _color;
    Color                   _bg_color;
};

#endif // NUMBER_DRAWER_H
