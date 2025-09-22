#include "PickleListenerContext.h"
#include "../IInputWithTimer/IInputWithTimer.h"
#include "../RemotePairingScreen/RemotePairingScreen.h"
#include "../PairingBlinker/PairingBlinker.h"
#include "../BlankBlinker/BlankBlinker.h"
#include "../ScoreboardBlinker/ScoreboardBlinker.h"
#include "../MatchWinBlinker/MatchWinBlinker.h"
#include "../IGameInput/IGameInput.h"

PickleListenerContext::PickleListenerContext(
    ScoreBoard* scoreboard_arg,
    GameObject* gameObj,
    GameState* gameSt,
    Reset* resetPtr,
    Inputs* inputsPtr,
    PinInterface* pinInterfacePtr,
    BlinkController* blinkControllerPtr,
    IInputWithTimer* inputWithTimerPtr,
    IInputWithTimer* sleepingInputWithTimer,
    IInputWithTimer* noBlinkInputWithTimer,
    IInputWithTimer* pairingInputWithTimer_arg,
    PairingBlinker* pairingBlinker,
    BlankBlinker* blankBlinker,
    MatchWinBlinker* matchWinBlinker,
    RemoteLocker* remoteLocker_arg,
    RemotePairingScreen* remotePairingScreen,
    ClockTimer* clockTimer,
    ClockUpdater* clockUpdater,
    bool& noScoreFlag
)   : scoreboard(scoreboard_arg),
      gameObject(gameObj),
      gameState(gameSt),
      reset(resetPtr),
      inputs(inputsPtr),
      pinInterface(pinInterfacePtr),
      blinkController(blinkControllerPtr),
      inputWithTimer(inputWithTimerPtr),
      sleepingInputWithTimer(sleepingInputWithTimer),
      noBlinkInputWithTimer(noBlinkInputWithTimer),
      pairingInputWithTimer(pairingInputWithTimer_arg),
      pairingBlinker(pairingBlinker),
      blankBlinker(blankBlinker),
      matchWinBlinker(matchWinBlinker),
      remoteLocker(remoteLocker_arg),
      remotePairingScreen( remotePairingScreen ),
      clockTimer(clockTimer),
      clockUpdater(clockUpdater),
      no_score(noScoreFlag) {
    print("[DEBUG_PAIR_SEG_FAULT] PickleListenerContext constructor called");
    print("[DEBUG_PAIR_SEG_FAULT] pairingInputWithTimer_arg: " << pairingInputWithTimer_arg);
    print("[DEBUG_PAIR_SEG_FAULT] this->pairingInputWithTimer: " << this->pairingInputWithTimer);
    print("[DEBUG_PAIR_SEG_FAULT] pairingBlinker: " << pairingBlinker);
    print("[DEBUG_PAIR_SEG_FAULT] remotePairingScreen: " << remotePairingScreen);
}
GameObject* PickleListenerContext::getGameObject() const { return gameObject; }
GameState* PickleListenerContext::getGameState() const { return gameState; }
Reset* PickleListenerContext::getReset() const { return reset; }
Inputs* PickleListenerContext::getInputs() const { return inputs; }
PinInterface* PickleListenerContext::getPinInterface() const { return pinInterface; }
BlinkController* PickleListenerContext::getBlinkController() const { return blinkController; }
IInputWithTimer* PickleListenerContext::getInputWithTimer() const { return inputWithTimer; }
IInputWithTimer* PickleListenerContext::getSleepingInputWithTimer() const { return sleepingInputWithTimer; }
IInputWithTimer* PickleListenerContext::getPairingInputWithTimer() const { return pairingInputWithTimer; }
RemotePairingScreen* PickleListenerContext::getRemotePairingScreen() const { return remotePairingScreen; }
PairingBlinker* PickleListenerContext::getPairingBlinker() const { return pairingBlinker; }
ScoreBoard* PickleListenerContext::getScoreboard() const { return scoreboard; }
bool& PickleListenerContext::getNoScoreFlag() const { return no_score; }
std::shared_ptr<MatchWinBlinker> PickleListenerContext::getMatchWinBlinker() const { 
    return std::shared_ptr<MatchWinBlinker>(matchWinBlinker, [](MatchWinBlinker*){});
}

// Thread safety
void PickleListenerContext::lock()   { mtx.lock(); }
void PickleListenerContext::unlock() { mtx.unlock(); }
IInputWithTimer* PickleListenerContext::getNoBlinkInputWithTimer() const { return noBlinkInputWithTimer; }
RemoteLocker* PickleListenerContext::getRemoteLocker() const { return remoteLocker; }
ClockTimer* PickleListenerContext::getClockTimer() const { return clockTimer; }
ClockUpdater* PickleListenerContext::getClockUpdater() const { return clockUpdater; }