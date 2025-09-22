#ifndef GameTimer_h
#define GameTimer_h
#include "../Arduino/Arduino.h"
class GameTimer {
public:
    GameTimer();
    void sleep_until(int milliseconds);
    static void gameDelay(int milliseconds);
    static unsigned long gameMillis();
    void start();
    unsigned long elapsed();
    ~GameTimer();

private:
    unsigned long _startTime;
};
#endif
