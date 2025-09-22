#ifndef REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H
#define REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H

#include "../TennisConstants/TennisConstants.h"
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class RegularGamePlayBeforeScoreState : public IRemoteListenerState {
public:
    void handleInput(PickleListenerContext &context) override;

private:
    void handleSelectionAndUpdate(PickleListenerContext &context, int selection);
    void showHelp();
};

#endif // REGULAR_GAME_PLAY_BEFORE_SCORE_STATE_H
