#ifndef REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
#define REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../RemoteLocker/RemoteLocker.h"
#include "../TranslateConstant/TranslateConstant.h"

class RegularGamePlayAfterScoreState : public IRemoteListenerState {
public:
    void handleInput(PickleListenerContext &context) override;

private:
    void showHelp();
};

#endif // REGULAR_GAME_PLAY_AFTER_SCORE_STATE_H
