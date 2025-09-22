#ifndef TEST_INPUT_WITH_TIMER_H
#define TEST_INPUT_WITH_TIMER_H

#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../IInputWithTimer/IInputWithTimer.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

struct TestCommand {
    enum Type { INPUT, PAUSE };
    Type type;
    int value;          // input code for INPUT type, pause duration in seconds for PAUSE type
};

class TestInputWithTimer : public IInputWithTimer {
public:
    TestInputWithTimer(unsigned long timeout_ms = 4000);
    TestInputWithTimer(Blinker* blinker, void* unused_inputs, unsigned long timeout_ms = 4000);
    ~TestInputWithTimer() override;
    int getInput() override;
private:
    Blinker* _blinker = nullptr;
    std::vector<TestCommand> _test_commands;
    size_t _current_index = 0;
    std::unordered_map<std::string, int> _input_map;
    void _loadTestInputs(const std::string& filename);
    bool _onRaspberryPi();
    std::chrono::steady_clock::time_point _start_time;
};

#endif // TEST_INPUT_WITH_TIMER_H

