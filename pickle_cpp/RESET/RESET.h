#ifndef RESET_h
#define RESET_h

#include "../Arduino/Arduino.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Team/Team.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../GameStateManager/GameStateManager.h"

class Reset {
public:
    Reset( PinInterface* pinInterface );
    ~Reset();
    void resetScoreboard( GameState* gameState );
    void resetPreviousValues();
    void refresh( GameState* gameState );

private:
    PinInterface*     _pinInterface;
    GameStateManager* _gameStateManager;
};

#endif
