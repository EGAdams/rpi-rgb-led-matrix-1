#ifndef IDISPLAY_H
#define IDISPLAY_H
#include <string>

class IDisplay {
public:
    virtual void setColor( const std::string& colorName ) = 0;
    virtual void drawText( const std::string& text, int x, int y ) = 0;
    virtual void clearScreen() = 0;
    virtual ~IDisplay() = default;
};


#endif // IDISPLAY_H
