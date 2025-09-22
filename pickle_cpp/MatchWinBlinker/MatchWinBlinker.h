#ifndef MATCH_WIN_BLINKER_H
#define MATCH_WIN_BLINKER_H

#include "../ScoreBoard/ScoreBoard.h"
#include "../GameTimer/GameTimer.h"
#include <iostream>
#include <memory>
#include <thread>
#include <atomic>

class MatchWinBlinker {
public:
    explicit MatchWinBlinker(ScoreBoard* scoreboard);
    ~MatchWinBlinker();

    void start();
    void stop();
    bool isRunning() const;

private:
    void blinkLoop();

    ScoreBoard* _scoreboard;
    std::thread _blinkThread;
    std::atomic<bool> _shouldStop;
    std::atomic<bool> _running;
};

#endif // MATCH_WIN_BLINKER_H
