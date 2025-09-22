Act as a World-class C++ programmer for the Teensy 4.1 microcontroller.
You are an expert at using the Arduino IDE for compiling sketches
You are an expert at refactoring C++ code.

# First Task
Analyze the following C++ Source code and think about where the reference to `_getttimeofday` might be.  Also think about where the timing is and how we could possibly change it out with a different timing approach.

Here is the source code for the BlinkController object that uses the standard C++ <chrono> lobrary:
## BlinkController.h
```cpp
#ifndef BLINKCONTROLLER_H
#define BLINKCONTROLLER_H

#include "PinInterface.h"
#include "GameState.h"
#include <chrono>

class BlinkController : public Observer {
public:
    explicit BlinkController( PinInterface* pinInterface, GameState* gameState );
    void update();

private:
    PinInterface* _pinInterface; // Interface for controlling pin output
    bool _toggledState = false; // Tracks the current toggle state (on/off)
    std::chrono::steady_clock::time_point _lastToggleTime; // Last toggle timestamp
};

#endif // BLINKCONTROLLER_H
```

## BlinkController.cpp
```cpp
// BlinkController.cpp

#include "../BlinkController/BlinkController.h"
#include "../PinInterface/PinInterface.h"
#include <chrono>

// Constructor
BlinkController::BlinkController( PinInterface* pinInterface, GameState* gameState )
    : _pinInterface( pinInterface ), _lastToggleTime( std::chrono::steady_clock::now()) {
        gameState->attach( this );
    }

void BlinkController::update() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - _lastToggleTime).count();

    // Toggle blink every 1 second
    if (elapsed >= 1) {
        _toggledState = !_toggledState; // Toggle the state
        _lastToggleTime = currentTime; // Reset the timer

        // Perform the actual blinking
        if (_toggledState) {
            // Turn ON the LEDs
            _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 1 );
            _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 1 );
            _pinInterface->pinDigitalWrite( 16, 1 ); // testing pins from top to bottom
            _pinInterface->pinDigitalWrite( 27, 1 );
            _pinInterface->pinDigitalWrite( 39, 1 );
            _pinInterface->pinDigitalWrite( 38, 1 );
            _pinInterface->pinDigitalWrite( 35, 1 );
            _pinInterface->pinDigitalWrite( 34, 1 );
        } else {
            // Turn OFF the LEDs
            _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 0 );
            _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 0 );
            _pinInterface->pinDigitalWrite( 16, 0 ); // testing pins from top to bottom
            _pinInterface->pinDigitalWrite( 27, 0 );
            _pinInterface->pinDigitalWrite( 39, 0 );
            _pinInterface->pinDigitalWrite( 38, 0 );
            _pinInterface->pinDigitalWrite( 35, 0 );
            _pinInterface->pinDigitalWrite( 34, 0 );
        }
    }
}
```



This object fails to compile on a Teensy 4.1 Arduino IDE compiler and here is the failure message:
```error
c:/program files (x86)/arduino/hardware/tools/arm/bin/../lib/gcc/arm-none-eabi/5.4.1/../../../../arm-none-eabi/lib/armv7e-m/fpu/fpv5-d16\libc.a(lib_a-gettimeofdayr.o): In function `_gettimeofday_r':

gettimeofdayr.c:(.text._gettimeofday_r+0x10): undefined reference to `_gettimeofday'

collect2.exe: error: ld returned 1 exit status

Error compiling for board Teensy 4.1.
```

# Second Task
Analyze the GameTimer C++ Object below and use it for the timing parts of the BlinkController Object. I do not want the BlinkController to use chrono since it will not compile.  GameTimer DOES compile, so I want the BlinkController object to use it instead.

## GameTimer.h
```cpp
#ifndef GameTimer_h
#define GameTimer_h
#include "Arduino.h"
class GameTimer {
 public:
    GameTimer();
    void start();
    unsigned long elapsed();
    static void gameDelay(int milliseconds);
    static unsigned long gameMillis();
    void sleep_until(int milliseconds);
    ~GameTimer();

 private:
    unsigned long _startTime;
};

#endif
```

## GameTimer.cpp
```cpp

#include "GameTimer.h"
#include "Arduino.h"
#if defined _WIN32 || defined _WIN64
#include <chrono>
#include <thread>
#endif

GameTimer::GameTimer() {}

void GameTimer::gameDelay( int milliseconds ) {
#if defined _WIN32 || defined _WIN64
    std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
#else
    delay( milliseconds );
#endif
}
GameTimer::~GameTimer() {}

void GameTimer::start() { _startTime = gameMillis(); }

unsigned long GameTimer::elapsed() { return gameMillis() - _startTime; }

void GameTimer::sleep_until( int milliseconds ) {
#if defined _WIN32 || defined _WIN64
    std::this_thread::sleep_until( std::chrono::system_clock::now() +
        std::chrono::milliseconds( milliseconds ) );
#else
    sleep_until( milliseconds );
#endif
}

unsigned long GameTimer::gameMillis() {
#if defined _WIN32 || defined _WIN64
    std::chrono::milliseconds ms =
        std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch() );
    return ms.count();
#else
    return millis();
#endif
}
```

# Third Task
Rewrite the BlinkerController object to use the GameTimer for timing.
