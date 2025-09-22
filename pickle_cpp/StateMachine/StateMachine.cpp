#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>  // Required for handling Ctrl+C (SIGINT)

#include "StateMachine.h"

// Global variable for tracking SIGINT (Ctrl+C) interrupts
volatile std::sig_atomic_t gSignalStatus = 0;

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 * @param signal The received signal.
 */
void signalHandler( int signal ) {
    if ( signal == SIGINT ) {
        gSignalStatus = 1;  // Set flag to indicate exit request
        std::cout << "\n[StateMachine] SIGINT received! Shutting down gracefully...\n";
    }
}

/**
 * @brief Constructs the StateMachine and initializes the starting state.
 * @param context Reference to the PickleListenerContext.
 */
StateMachine::StateMachine( PickleListenerContext& context )
    : _context( context ), _currentStateId( PAIRING_MODE_STATE ) {

    // Register signal handler for SIGINT
    std::signal( SIGINT, signalHandler );

    setState( PAIRING_MODE_STATE );  // Start in pairing mode
}

/**
 * @brief Runs the state machine loop, processing input and transitioning states.
 */
void StateMachine::run() {
    while ( _context.getGameState()->gameRunning() && gSignalStatus != 1 ) {
        // print( "before state machine delay: " << _state_machine_delay << "ms..." ); // 021225
        std::this_thread::sleep_for( std::chrono::milliseconds( _state_machine_delay )); // sleep for 500 milliseconds
        // print( "after state machine delay of: " << _state_machine_delay << "ms." );
        _currentState->handleInput( _context );                 // Execute the logic for the current state
        int nextState = _context.getGameState()->getGamePlayState();    // Transition to a new state if necessary
        StateTranslator stateTranslator;
        std::string stateName = stateTranslator.translateState(nextState);
        print( "========================================================================" );
        print( "=== setting state in state machine to " << stateName << " ===");
        print( "========================================================================\n\n\n" );
        if ( nextState != _currentStateId ) {
            setState( nextState );
        }
    }
    // std::cout << "[StateMachine] Exiting state machine loop.\n";
}

/**
 * @brief Transitions the state machine to a new state.
 * @param newState The new state identifier.
 */
void StateMachine::setState( int newState ) {
    _currentStateId = newState;
    _currentState = createState( newState );
    StateTranslator stateTranslator;
    std::string stateName = stateTranslator.translateState(newState);
    std::cout << "\n\n\n****************************************" << std::endl;
    std::cout << "\n\n\n[StateMachine] Transitioned to state: [" << stateName << "]" << std::endl;
    std::cout << "\n\n\n****************************************\n\n\n" << std::endl;
}

/**
 * @brief Creates the appropriate state object based on the game action.
 * @param currentState The state representing the current state of the game.
 * @return A unique pointer to the corresponding state object.
 */
std::unique_ptr<IRemoteListenerState> StateMachine::createState( int currentState ) {
    switch ( currentState ) {
    case PAIRING_MODE_STATE:
        return std::make_unique<PairingModeState>();
    case PAIRING_SLEEP_MODE_STATE:
    case NO_SCORE_SLEEP_STATE:
    case REGULAR_PLAY_SLEEP_STATE:
        std::cout << "transitioning to sleep mode state... " << std::endl;
        return std::make_unique<SleepModeState>();
    case REGULAR_PLAY_NO_SCORE_STATE:
        return std::make_unique<RegularGamePlayBeforeScoreState>();
    case REGULAR_PLAY_AFTER_SCORE_STATE:
        return std::make_unique<RegularGamePlayAfterScoreState>();
    case AFTER_MATCH_WIN_STATE:
        print( "*** Entering AFTER_MATCH_WIN_STATE ***" );
        return std::make_unique<AfterMatchWinState>();
    case MATCH_WIN_BLINK_STATE:
        print( "*** Entering MATCH_WIN_BLINK_STATE ***" );
        return std::make_unique<MatchWinBlinkState>();
    // case UNDO_STATE:
    //     print( "*** Entering UNDO_STATE ***" );
    //     return std::make_unique<UndoState>();
    // case INTRODUCTION_SCREEN_STATE:
    //     print( "*** Entering INTRODUCTION_SCREEN_STATE ***" );
    //     return std::make_unique<IntroductionScreenState>();
    default:
        std::cout << "[StateMachine] WARNING: Unknown state (" << currentState 
        << "), defaulting to REGULAR_PLAY_AFTER_SCORE_STATE." << std::endl;
        return std::make_unique<RegularGamePlayAfterScoreState>();
    }
}
