#ifndef PAIRING_MODE_STATE_H
#define PAIRING_MODE_STATE_H

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include "../TennisConstants/TennisConstants.h"

/***************************************************************
 * PairingModeState
 *
 * Handles logic specific to the pairing process. It listens 
 * for input from the remote, ensures both players are paired,
 * and transitions the game to sleep mode once pairing is complete.
 ***************************************************************/
class PairingModeState : public IRemoteListenerState {
public:
    void handleInput( PickleListenerContext &context ) override;
};

#endif // PAIRING_MODE_STATE_H
