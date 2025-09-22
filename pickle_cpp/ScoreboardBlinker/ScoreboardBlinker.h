#ifndef SCOREBOARD_BLINKER_H
#define SCOREBOARD_BLINKER_H

#include <atomic>
#include <thread>
#include "../ScoreBoard/ScoreBoard.h"
#include "../Blinker/Blinker.h"

class ScoreboardBlinker : public Blinker {
private:
    std::atomic<bool> should_stop{false};
    std::thread blink_thread;
    ScoreBoard* _scoreboard;
    unsigned long _sleep_start;

    void blinkLoop();
    void blinkTennisBall(bool show);

public:
    ScoreboardBlinker( ScoreBoard* scoreBoard );
    ~ScoreboardBlinker();
    void start() override;
    void stop()  override;
};

#endif // SCOREBOARD_BLINKER_H

