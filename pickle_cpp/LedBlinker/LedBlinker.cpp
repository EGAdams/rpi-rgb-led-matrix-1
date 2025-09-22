#include "LedBlinker.h"

LedBlinker::LedBlinker( int timeOn, int timeOff )
    : _timeOn( timeOn ), _timeOff( timeOff ), _toggledState( false ), _lastToggleTime( 0 ) {}

bool LedBlinker::update() {
    int currentTime = GameTimer::gameMillis();
    int interval = _toggledState ? _timeOff : _timeOn;
    if (currentTime - _lastToggleTime >= interval) {
        _toggledState = !_toggledState;
        _lastToggleTime = currentTime;
        return true; // Indicates the LED state changed
    }
    return false; // Indicates no change in LED state
}
