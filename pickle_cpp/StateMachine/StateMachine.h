#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include "../TranslateConstant/TranslateConstant.h"
#include "../StateTranslator/StateTranslator.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../IRemoteListenerState/IRemoteListenerState.h"
#include "../PairingModeState/PairingModeState.h"
#include "../SleepModeState/SleepModeState.h"
#include "../RegularGamePlayBeforeScoreState/RegularGamePlayBeforeScoreState.h"
#include "../RegularGamePlayAfterScoreState/RegularGamePlayAfterScoreState.h"
#include "../GameState/GameState.h"
#include "../AfterMatchWinState/AfterMatchWinState.h"
#include "../MatchWinBlinkState/MatchWinBlinkState.h"
// #include "../UndoState/UndoState.h"
// #include "../IntroductionScreenState/IntroductionScreenState.h"
#include "../PickleListenerContext/PickleListenerContext.h"

/**
 * @class StateMachine
 * @brief Manages the different game states using the State Pattern.
 *
 * The StateMachine class encapsulates:
 *  - The current state of the system
 *  - The logic for transitioning between states
 *  - Execution of the current state's behavior
 */
class StateMachine {
public:
  /**
   * @brief Constructs the StateMachine with the given context.
   * @param context Reference to the PickleListenerContext.
   */
  explicit StateMachine( PickleListenerContext& context );

  /**
   * @brief Runs the state machine, executing the current state in a loop.
   */
  void run();

private:
  PickleListenerContext& _context;
  std::unique_ptr<IRemoteListenerState> _currentState;
  int _currentStateId;
  int _state_machine_delay = 100; // while loop delay in milliseconds

  /**
   * @brief Sets the current state based on the game state.
   * @param newState The new state to transition into.
   */
  void setState( int newState );

  /**
   * @brief Creates the appropriate state object based on the current action.
   * @param currentAction The action representing the current state.
   * @return A unique pointer to the new state object.
   */
  std::unique_ptr<IRemoteListenerState> createState( int currentAction );
};

#endif // STATE_MACHINE_H
