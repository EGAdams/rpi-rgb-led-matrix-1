#ifndef CONSOLEDISPLAY_H
#define CONSOLEDISPLAY_H

#include <iostream>
#include <string>
#include "../IDisplay/IDisplay.h"
#include "../ColorManager/ColorManager.h"

class ConsoleDisplay : public IDisplay {
private:
  ColorManager* _colorManager;
  std::string   _current_color_string;  // Store the colorName

public:
  ConsoleDisplay( ColorManager* colorManager );
  ~ConsoleDisplay() override = default;

  void setColor( const std::string& colorName ) override;
  void drawText( const std::string& text, int x, int y ) override;
  void clearScreen() override;
};
#endif // CONSOLEDISPLAY_H
