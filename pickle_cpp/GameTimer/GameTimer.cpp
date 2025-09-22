#include "../GameTimer/GameTimer.h"
#include "../Arduino/Arduino.h"
#if !defined(ARDUINO_TEENSY41)
#include <chrono>
#include <thread>
#endif

GameTimer::GameTimer() {}

void GameTimer::gameDelay( int milliseconds ) {
#if !defined(ARDUINO_TEENSY41)
    // std::cout << "\n\n************************************" << std::endl;
    // std::cout << "*** sleeping " << milliseconds << " millisseconds... ***" << std::endl;
    // std::cout << "************************************\n\n" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
#else
    delay( milliseconds );
#endif
}
GameTimer::~GameTimer() {}

void GameTimer::start() { _startTime = gameMillis(); }

unsigned long GameTimer::elapsed() { return gameMillis() - _startTime; }

void GameTimer::sleep_until( int milliseconds ) {
#if !defined(ARDUINO_TEENSY41)
    std::this_thread::sleep_until( std::chrono::system_clock::now() +
        std::chrono::milliseconds( milliseconds ) );
#else
    sleep_until( milliseconds );
#endif
}

unsigned long GameTimer::gameMillis() {
#if !defined(ARDUINO_TEENSY41)
    std::chrono::milliseconds ms =
        std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch() );
    return ms.count();
#else
    return millis();
#endif
}
