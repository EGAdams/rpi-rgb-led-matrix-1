#ifndef BLANK_BLINKER_H
#define BLANK_BLINKER_H

#include "../Blinker/Blinker.h"
#include <thread>
#include <atomic>
#include <iostream>

class BlankBlinker : public Blinker {
public:
    BlankBlinker() : _should_stop(false) { }
    ~BlankBlinker() { stop(); }

    void start() override {
        _should_stop = false;
        _thread = std::thread(&BlankBlinker::threadLoop, this);
    }

    void stop() override {
        _should_stop = true;
        if (_thread.joinable()) {
            _thread.join();
        }
    }

private:
    void threadLoop() {
        // Minimal thread function—just exits immediately
        return;
    }

    std::thread _thread;
    std::atomic<bool> _should_stop;
};

#endif

