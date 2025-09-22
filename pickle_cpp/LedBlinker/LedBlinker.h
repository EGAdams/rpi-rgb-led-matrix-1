#ifndef LED_BLINKER_H
#define LED_BLINKER_H

#include "../GameTimer/GameTimer.h"

class LedBlinker {
public:
    LedBlinker( int timeOn, int timeOff );
    bool update();

private:
    int _timeOn, _timeOff;
    bool _toggledState;
    bool _lastToggleTime;
};

#endif