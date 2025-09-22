#ifndef PICKLE_LISTENER_CONTEXT_H
#define PICKLE_LISTENER_CONTEXT_H

#include <mutex>
#include <memory>
#include "../GameObject/GameObject.h"
#include "../GameState/GameState.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../RESET/RESET.h"
#include "../INPUTS/INPUTS.h"
#include "../PinInterface/PinInterface.h"
#include "../BlinkController/BlinkController.h"
#include "../IInputWithTimer/IInputWithTimer.h"
#include "../PairingBlinker/PairingBlinker.h"
#include "../BlankBlinker/BlankBlinker.h"
#include "../RemoteLocker/RemoteLocker.h"
#include "../RemotePairingScreen/RemotePairingScreen.h"
#include "../MatchWinBlinker/MatchWinBlinker.h"
#include "../ClockTimer/ClockTimer.h"
#include "../ClockUpdater/ClockUpdater.h"

/************************************************************
 * PickleListenerContext
 *
 * This class provides a shared context for all game states
 * within the PickleBall state machine. It manages shared
 * resources like game objects, input handlers, and scoreboard.
 *
 * Key Features:
 * - Thread-Safe: Uses explicit lock() and unlock() methods.
 * - Encapsulated Access: Provides getters for all shared resources.
 * - Centralized State Management: Holds references to the game state
 *   and relevant inputs required by different states.
 ************************************************************/
class PickleListenerContext {
public:
    PickleListenerContext(
        ScoreBoard*           scoreboard,
        GameObject*           gameObj,
        GameState*            gameSt,
        Reset*                resetPtr,
        Inputs*               inputsPtr,
        PinInterface*         pinInterfacePtr,
        BlinkController*      blinkControllerPtr,
        IInputWithTimer*      inputWithTimerPtr,
        IInputWithTimer*      sleepingInputWithTimer,
        IInputWithTimer*      noBlinkInputWithTimer,
        IInputWithTimer*      pairingInputWithTimer,
        PairingBlinker*       pairingBlinker,
        BlankBlinker*         blankBlinker,
        MatchWinBlinker*      matchWinBlinker,
        RemoteLocker*         remoteLocker,
        RemotePairingScreen*  remotePairingScreen,
        ClockTimer*           clockTimer,
        ClockUpdater*         clockUpdater,
        bool&                 noScoreFlag
    );

    // Accessors for Shared Resources
    GameObject* getGameObject() const;
    GameState* getGameState() const;
    Reset* getReset() const;
    Inputs* getInputs() const;
    PinInterface* getPinInterface() const;
    BlinkController* getBlinkController() const;
    IInputWithTimer* getInputWithTimer() const;
    IInputWithTimer* getNoBlinkInputWithTimer() const;
    IInputWithTimer* getSleepingInputWithTimer() const;
    IInputWithTimer* getPairingInputWithTimer() const;
    RemotePairingScreen* getRemotePairingScreen() const;
    PairingBlinker* getPairingBlinker() const;
    std::shared_ptr<MatchWinBlinker> getMatchWinBlinker() const;
    RemoteLocker* getRemoteLocker() const;
    ScoreBoard* getScoreboard() const;
    ClockTimer* getClockTimer() const;
    ClockUpdater* getClockUpdater() const;
    bool& getNoScoreFlag() const;

    // Thread Safety Controls
    void lock();
    void unlock();

private:
    ScoreBoard* scoreboard;
    GameObject* gameObject;
    GameState* gameState;
    Reset* reset;
    Inputs* inputs;
    PinInterface* pinInterface;
    BlinkController* blinkController;
    IInputWithTimer* inputWithTimer;
    IInputWithTimer* sleepingBlinker;
    IInputWithTimer* sleepingInputWithTimer;
    IInputWithTimer* noBlinkInputWithTimer;
    IInputWithTimer* pairingInputWithTimer;
    PairingBlinker* pairingBlinker;
    BlankBlinker* blankBlinker;
    MatchWinBlinker* matchWinBlinker;
    RemoteLocker*   remoteLocker;
    RemotePairingScreen* remotePairingScreen;
    ClockTimer* clockTimer;
    ClockUpdater* clockUpdater;
    bool& no_score;
    std::mutex mtx;
};

#endif // PICKLE_LISTENER_CONTEXT_H
