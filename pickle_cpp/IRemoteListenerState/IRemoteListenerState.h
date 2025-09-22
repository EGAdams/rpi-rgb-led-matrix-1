/************************************************************
 * IRemoteListenerState.h
 *
 * Interface for the State Pattern used in the Remote Listener.
 * Each game state (PairingMode, SleepMode, RegularGamePlay, etc.)
 * will implement this interface.
 ************************************************************/

#ifndef IREMOTE_LISTENER_STATE_H
#define IREMOTE_LISTENER_STATE_H

#include "../PickleListenerContext/PickleListenerContext.h"

class IRemoteListenerState {
public:
    virtual ~IRemoteListenerState() = default;

    /**
     * Handles input and state transitions for the given state.
     * @param context A reference to the PickleListenerContext containing shared resources.
     */
    virtual void handleInput( PickleListenerContext &context ) = 0;
};

#endif // IREMOTE_LISTENER_STATE_H
