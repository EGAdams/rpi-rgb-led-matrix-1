#ifndef CLOCK_UPDATER_H
#define CLOCK_UPDATER_H

#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include "../ClockTimer/ClockTimer.h"

class ScoreBoard; // Forward declaration

class ClockUpdater {
public:
    ClockUpdater(ClockTimer* clockTimer, ScoreBoard* scoreboard );
    ~ClockUpdater();
    
    void start();
    void stop();
    bool isRunning() const;
    
    // Set the scoreboard for clock updates
    void setScoreBoard(ScoreBoard* scoreBoard);

private:
    ClockTimer* _clockTimer;
    ScoreBoard* _scoreBoard;
    std::atomic<bool> _shouldStop;
    std::atomic<bool> _isRunning;
    std::thread _updateThread;
    
    void _updateLoop();
};

#endif