#ifndef REMOTE_INPUT_WITH_TIMER_H
#define REMOTE_INPUT_WITH_TIMER_H

#include <memory>
#include <cstdint>
#include <unordered_map>
#include <string>
#include "../Blinker/Blinker.h"
#include "../GameTimer/GameTimer.h"
#include "../INPUTS/INPUTS.h"
#include "../IInputWithTimer/IInputWithTimer.h"

class RemoteInputWithTimer : public IInputWithTimer {
private:
  Blinker* _blinker;  // Use shared_ptr to prevent dangling pointers
  Inputs*  _inputs;   // Assuming Inputs* is managed externally
  std::unordered_map<std::string, int> _input_map;

public:
    RemoteInputWithTimer( Blinker* blinker, Inputs* inputs, unsigned long timeout_ms = 4000 );
    ~RemoteInputWithTimer() override;

    int getInput() override;
};

#endif // REMOTE_INPUT_WITH_TIMER_H

