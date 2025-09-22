#ifndef SLEEP_MODE_STATE_H
#define SLEEP_MODE_STATE_H

#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include "../GameTimer/GameTimer.h"

class SleepModeState : public IRemoteListenerState {

public:
    void handleInput(PickleListenerContext &context) override;

private:
    void _stop_the_clock( PickleListenerContext& context );

};

#endif // SLEEP_MODE_STATE_H
