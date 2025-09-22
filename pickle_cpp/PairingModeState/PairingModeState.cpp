#include "PairingModeState.h"
#include <cassert>
#include <stdexcept>

/***************************************************************
 * PairingModeState::handleInput()
 *
 * - Listens for player inputs to pair players.
 * - If a valid input is received, marks the corresponding player as paired.
 * - If both players are paired or pairing times out, transitions to sleep mode.
 ***************************************************************/
void PairingModeState::handleInput( PickleListenerContext& context ) {
    // print( "[DEBUG_PAIR_SEG_FAULT] Entering PairingModeState::handleInput()" );
    
    try {
        // print( "[DEBUG_PAIR_SEG_FAULT] Attempting to lock context" );
        context.lock();  // Ensure thread safety
        // print( "[DEBUG_PAIR_SEG_FAULT] Context locked successfully" );
        
        print( "============================" );
        print( "=== [STATE: PairingMode] ===" );
        print( "============================\n" );

        // **Null Safety Checks**
        // print( "[DEBUG_PAIR_SEG_FAULT] Checking remotePairingScreen..." );
        auto remotePairingScreen = context.getRemotePairingScreen();
        if ( !remotePairingScreen ) {
            print( "*** ERROR: remotePairingScreen is NULL! ***" );
            context.unlock();
            return;
        }
        // print( "[DEBUG_PAIR_SEG_FAULT] remotePairingScreen is valid" );

        // print( "[DEBUG_PAIR_SEG_FAULT] Checking pairingBlinker..." );
        auto pairingBlinker = context.getPairingBlinker();
        if ( !pairingBlinker ) {
            print( "*** ERROR: pairingBlinker is NULL! ***" );
            context.unlock();
            return;
        }
        // print( "[DEBUG_PAIR_SEG_FAULT] pairingBlinker is valid" );
        
        // print( "[DEBUG_PAIR_SEG_FAULT] Checking pairingInputWithTimer..." );
        auto pairingInputWithTimer = context.getPairingInputWithTimer();
        if ( !pairingInputWithTimer ) {
            print( "*** ERROR: pairingInputWithTimer is NULL! ***" );
            context.unlock();
            return;
        }
        // print( "[DEBUG_PAIR_SEG_FAULT] pairingInputWithTimer is valid" );
        
        // Additional assertions
        assert( remotePairingScreen != nullptr );
        assert( pairingBlinker != nullptr );
        assert( pairingInputWithTimer != nullptr );
        // print( "[DEBUG_PAIR_SEG_FAULT] All assertions passed" );

        // Ensure we are in pairing mode and the blinker is active
        // print( "[DEBUG_PAIR_SEG_FAULT] Checking pairing mode and blinker state..." );
        bool inPairingMode = remotePairingScreen->inPairingMode();
        bool blinkerAwake = pairingBlinker->awake();
        // print( "[DEBUG_PAIR_SEG_FAULT] inPairingMode: " << (inPairingMode ? "true" : "false") );
        // print( "[DEBUG_PAIR_SEG_FAULT] blinkerAwake: " << (blinkerAwake ? "true" : "false") );
        
        while ( inPairingMode && blinkerAwake ) {
            // print( "[DEBUG_PAIR_SEG_FAULT] Entering pairing loop" );
            print( "Waiting for pairing input..." );
            
            // print( "[DEBUG_PAIR_SEG_FAULT] About to call getInput() on pairingInputWithTimer" );
            int selection = -1;
            try {
                selection = pairingInputWithTimer->getInput(); // the one with the pairing blinker
                print( "[DEBUG_PAIR_SEG_FAULT] getInput() returned: " << selection );
            } catch (const std::exception& e) {
                print( "[DEBUG_PAIR_SEG_FAULT] Exception in getInput(): " << e.what() );
                context.unlock();
                throw;
            } catch (...) {
                print( "[DEBUG_PAIR_SEG_FAULT] Unknown exception in getInput()" );
                context.unlock();
                throw;
            }
            
            print( "[DEBUG_PAIR_SEG_FAULT] Processing selection: " << selection );
            if ( selection == GREEN_REMOTE_GREEN_SCORE ) {
                print( "[DEBUG_PAIR_SEG_FAULT] Green player pairing..." );
                remotePairingScreen->greenPlayerPressed();
                pairingBlinker->setGreenPlayerPaired( true );
                print( "Green player paired." );
            } else if ( selection == RED_REMOTE_RED_SCORE ) {
                print( "[DEBUG_PAIR_SEG_FAULT] Red player pairing..." );
                remotePairingScreen->redPlayerPressed();
                pairingBlinker->setRedPlayerPaired( true );
                print( "Red player paired." );
            } else if ( selection == INPUT_TIMEOUT_CODE ) {
                print( "Pairing timeout. Clearing pairing screen and transitioning to sleep mode..." );
                pairingBlinker->stop();
                context.getScoreboard()->clearScreen();
                context.getGameState()->setCurrentAction( SLEEP_MODE );
                context.getGameState()->setGamePlayState( PAIRING_SLEEP_MODE_STATE );
                context.unlock();
                return;
            } else {
                print( "*** Invalid selection during pairing: " << selection << " ***" );
                std::this_thread::sleep_for( std::chrono::milliseconds( 500 ));
            }

            // If both players are paired, exit the loop
            // print( "[DEBUG_PAIR_SEG_FAULT] Checking if both players paired..." );
            if ( pairingBlinker->areBothPlayersPaired()) {
                print( "Both players paired, exiting pairing mode." );
                // context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );
                context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE           );
                context.getGameState()->setGamePlayState( REGULAR_PLAY_NO_SCORE_STATE   );
                break;
            }
            
            // Update loop conditions
            // print( "[DEBUG_PAIR_SEG_FAULT] Updating loop conditions..." );
            inPairingMode = remotePairingScreen->inPairingMode();
            blinkerAwake = pairingBlinker->awake();
            // print( "[DEBUG_PAIR_SEG_FAULT] Updated - inPairingMode: " << (inPairingMode ? "true" : "false") << ", blinkerAwake: " << (blinkerAwake ? "true" : "false") );
        }

        // If pairing is complete or timeout occurs, transition to sleep mode
        // print( "[DEBUG_PAIR_SEG_FAULT] Checking final blinker state..." );
        if ( !pairingBlinker->awake()) {
            print( "Pairing blinker is not awake, stopping pairing mode..." );
            pairingBlinker->stop();
            print( "Clearing pairing screen and transitioning to sleep mode..." );
            context.getScoreboard()->clearScreen();
            context.getGameState()->setCurrentAction( SLEEP_MODE );
            context.getGameState()->setGamePlayState( PAIRING_SLEEP_MODE_STATE );
        }

        // print( "[DEBUG_PAIR_SEG_FAULT] Unlocking context before exit" );
        context.unlock();  // Unlock before exiting
        // print( "[DEBUG_PAIR_SEG_FAULT] PairingModeState::handleInput() completed successfully" );
    } catch (const std::exception& e) {
        // print( "[DEBUG_PAIR_SEG_FAULT] Exception in handleInput(): " << e.what() );
        try {
            context.unlock();
        } catch (...) {
            // print( "[DEBUG_PAIR_SEG_FAULT] Failed to unlock context in exception handler" );
        }
        throw;
    } catch (...) {
        // print( "[DEBUG_PAIR_SEG_FAULT] Unknown exception in handleInput()" );
        try {
            context.unlock();
        } catch (...) {
            // print( "[DEBUG_PAIR_SEG_FAULT] Failed to unlock context in exception handler" );
        }
        throw;
    }
}
