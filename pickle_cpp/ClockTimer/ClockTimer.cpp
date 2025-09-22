#include "ClockTimer.h"

ClockTimer::ClockTimer() : _startTime(), _elapsedTime(0), _isRunning(false) {
    std::cout << "ClockTimer constructor called" << std::endl;
}

ClockTimer::~ClockTimer() {
    std::cout << "ClockTimer destructor called" << std::endl;
}

void ClockTimer::start() {
    if (!_isRunning) {
        _startTime = std::chrono::steady_clock::now();
        _isRunning = true;
        std::cout << "ClockTimer started" << std::endl;
    }
}

void ClockTimer::stop() {
    if (_isRunning) {
        auto endTime = std::chrono::steady_clock::now();
        _elapsedTime += endTime - _startTime;
        _isRunning = false;
        std::cout << "ClockTimer stopped. Total elapsed: " << getElapsedSeconds() << " seconds" << std::endl;
    }
}

void ClockTimer::reset() {
    _elapsedTime = std::chrono::duration<double>(0);
    _isRunning = false;
    std::cout << "ClockTimer reset" << std::endl;
}

int ClockTimer::getElapsedSeconds() const {
    std::chrono::duration<double> totalTime = _elapsedTime;
    
    if (_isRunning) {
        auto currentTime = std::chrono::steady_clock::now();
        totalTime += currentTime - _startTime;
    }
    
    return static_cast<int>(totalTime.count());
}

void ClockTimer::setElapsedSeconds( std::chrono::duration<double> elapsed_seconds ) { _elapsedTime = elapsed_seconds; }


bool ClockTimer::isRunning() const {
    return _isRunning;
}