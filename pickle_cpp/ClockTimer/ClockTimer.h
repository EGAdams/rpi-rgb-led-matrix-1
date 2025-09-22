#ifndef CLOCK_TIMER_H
#define CLOCK_TIMER_H

#include <chrono>
#include <iostream>

class ClockTimer {
public:
    ClockTimer();
    ~ClockTimer();
    
    void start();
    void stop();
    void reset();
    int getElapsedSeconds() const;
    void setElapsedSeconds( std::chrono::duration<double> elapsed_time );
    bool isRunning() const;

private:
    std::chrono::steady_clock::time_point _startTime;
    std::chrono::duration<double> _elapsedTime;
    bool _isRunning;
};

#endif