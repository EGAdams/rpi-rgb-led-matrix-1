
#ifndef ArduinoObject_h
#define ArduinoObject_h

#include <iostream>
#include <string>
#define OUTPUT 0
#define LOW 0
#define HIGH 1
#include "../GameTimer/GameTimer.h"

class ArduinoObject {
 public:
  ArduinoObject();
  ~ArduinoObject();
  void gameDelay(int timeToDelay);
  int gameMillis();
  // static void printLog( std::string message );
};

#endif
