#ifndef AFTER_MATCH_WIN_STATE_H
#define AFTER_MATCH_WIN_STATE_H

#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class AfterMatchWinState : public IRemoteListenerState {
public:
  void handleInput( PickleListenerContext& context ) override;

private:
  void handleSelectionAndUpdate( PickleListenerContext& context, int selection );
  void showHelp();
  void _stop_the_clock( PickleListenerContext& context );
};

#endif // AFTER_MATCH_WIN_STATE_H
