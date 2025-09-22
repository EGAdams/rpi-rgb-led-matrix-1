/*
 * After Match Win State
 */
#include "AfterMatchWinState.h"

void AfterMatchWinState::handleInput( PickleListenerContext& context ) {
    context.lock();  // <---------------------- lock the context ------------------------<<
    print( "===================================" );
    print( "=== [STATE: AfterMatchWinState] ===" );
    print( "===================================\n\n\n" );

    if ( context.getClockTimer() != nullptr && context.getClockTimer()->isRunning()) {
        context.getClockTimer()->stop();
        print( "*** ClockTimer stopped for AFTER_MATCH_WIN_STATE ***" );
    }

    if ( context.getClockUpdater() != nullptr && context.getClockUpdater()->isRunning()) {
        context.getClockUpdater()->stop();
        print( "*** ClockUpdater thread stopped for AFTER_MATCH_WIN_STATE ***" );
    }

    context.getNoBlinkInputWithTimer()->setTimeout( AFTER_MATCH_UNDO_TIMEOUT_MS );
    print( "Using noBlinkInputWithTimer with 15-second timeout for blinking ball and UNDO detection..." );
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer for blinking ball
    print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ));
    unsigned long timeElapsed = context.getNoBlinkInputWithTimer()->getTimeSlept();
    print( "Time elapsed in AfterMatchWinState: " + std::to_string( timeElapsed ) + "ms" );

    if ( selection == INPUT_TIMEOUT_CODE ) {
            print( "*** UNDO after 15-second window - resetting match and starting fresh ***" );
            _stop_the_clock( context ); // Stop the clock timer when entering AFTER_MATCH_WIN_STATE
            context.getGameObject()->history()->clear();
            context.getGameObject()->resetMatch();
            print( "setting state to NO_SCORE_SLEEP_STATE..." );
            context.getGameState()->setState( NO_SCORE_SLEEP_STATE );
            context.getGameState()->setGamePlayState( NO_SCORE_SLEEP_STATE );
            context.unlock();
            return;
    }

    if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO || selection == UMPIRE_REMOTE_UNDO ) {
        if ( timeElapsed <= AFTER_MATCH_UNDO_TIMEOUT_MS ) {
            print( "*** UNDO within 15-second window - executing undo ***" );
            // context.getGameObject()->undo(); // this only prints.  082125
            std::cout << "*** DEBUG: Processing UNDO command ***" << std::endl;
            GameStateManager* gameStateManager = new GameStateManager( context.getGameObject()->getScoreBoard());
            Team* team_a = context.getGameObject()->getTeamA();
            Team* team_b = context.getGameObject()->getTeamB();
            auto history = context.getGameObject()->history(); // Get shared_ptr
            
            std::cout << "*** DEBUG: Created GameStateManager and retrieved objects from context ***" << std::endl;
            std::cout << "*** DEBUG: About to call setPreviousGameState ***" << std::endl;
            
            gameStateManager->setPreviousGameState( context.getGameState(), team_a, team_b, *history );
            
            std::cout << "*** DEBUG: setPreviousGameState completed successfully ***" << std::endl;
            context.getGameState()->setPlayerButton( 0 );

            context.getGameObject()->getScoreBoard()->update();  // dont forget the update after the game state change.
            
         
            delete gameStateManager;                             // Clean up the locally created C++ Objects
            // Note: team_a and team_b are managed by GameObject, don't delete them
            // Note: history is now a shared_ptr, don't delete it

            context.getClockTimer()->setElapsedSeconds( context.getGameState()->getMatchWinTime());

            // Start the clock timer when transitioning back to REGULAR_PLAY_AFTER_SCORE_STATE state
            if ( context.getClockTimer() != nullptr ) {
                context.getClockTimer()->start();
                print( "*** ClockTimer started for undo transition ***" );

                // Set the ClockTimer in the ScoreBoard so it can display elapsed time
                if ( context.getScoreboard() != nullptr ) {
                    context.getScoreboard()->setClockTimer( context.getClockTimer());
                    print( "*** ClockTimer set in ScoreBoard for elapsed time display (undo) ***" );
                }
            }
            
             // start the clock updater thread when entering REGULAR_PLAY_AFTER_SCORE_STATE
            if ( context.getClockUpdater() != nullptr && context.getClockUpdater()->isRunning()) {
                context.getClockUpdater()->stop();
                print( "*** ClockUpdater thread stopped for AFTER_MATCH_WIN_STATE ***" );
            }

            print( "setting state to REGULAR_PLAY_AFTER_SCORE_STATE..." );
            context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
            context.getGameState()->setGamePlayState( REGULAR_PLAY_AFTER_SCORE_STATE );
            std::chrono::duration<double> last_match_win_time = context.getGameState()->getMatchWinTime();
            if ( last_match_win_time == std::chrono::duration<double>::zero() ) {
                _stop_the_clock( context );
            }
            context.unlock();
            return;
        } else {
            print( "*** UNDO after 15-second window - resetting match and starting fresh ***" );
            _stop_the_clock( context ); // Stop the clock timer when entering NO_SCORE_SLEEP_STATE
            context.getGameObject()->history()->clear();
            context.getGameObject()->resetMatch();
            context.getScoreboard()->update();
            print( "setting state to REGULAR_PLAY_NO_SCORE_STATE..." );
            context.getGameState()->setState( NO_SCORE_SLEEP_STATE );
            context.getGameState()->setGamePlayState( NO_SCORE_SLEEP_STATE );
            context.unlock();
            return;
        }
    }

    // Handle score buttons (GREEN/RED team score buttons)
    if ( selection == GREEN_REMOTE_GREEN_SCORE ||
         selection == GREEN_REMOTE_RED_SCORE ||
         selection == RED_REMOTE_GREEN_SCORE ||
         selection == RED_REMOTE_RED_SCORE ||
         selection == UMPIRE_REMOTE_GREEN_SCORE ||
         selection == UMPIRE_REMOTE_RED_SCORE ) {
        // Any score button press resets match and goes to Regular Play Before Score
        print( "*** Score button pressed - resetting match and starting fresh ***" );
        context.getGameObject()->resetMatch();
        context.getScoreboard()->update();
        print( "setting state to REGULAR_PLAY_NO_SCORE_STATE..." );
        context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );
        context.getGameState()->setGamePlayState( REGULAR_PLAY_NO_SCORE_STATE );
        _stop_the_clock( context );
        context.unlock();
        return;
    }

   
    if ( timeElapsed > AFTER_MATCH_UNDO_TIMEOUT_MS ) { // any other button press after 15-second timeout
        print( "*** Any button pressed after 15-second window - resetting match and starting fresh ***" );
        context.getGameObject()->resetMatch();
        print( "setting state to REGULAR_PLAY_NO_SCORE_STATE..." );
        context.getGameState()->setState( REGULAR_PLAY_NO_SCORE_STATE );
        context.getGameState()->setGamePlayState( REGULAR_PLAY_NO_SCORE_STATE );
        _stop_the_clock( context );
        context.getScoreboard()->update();
        context.unlock();
        return;
    } else {
        print( "*** Invalid input within 15-second window, ignoring... ***" );
        context.unlock();
        return;
    }
}

void AfterMatchWinState::handleSelectionAndUpdate( PickleListenerContext& context, int selection ) {
    print( "Handling selection: " + std::to_string( selection ));
}

void AfterMatchWinState::showHelp() {
    print( "AfterMatchWinState Help:" );
    print( "- Green/Red Score buttons: Start new match" );
    print( "- Undo button: Return to previous state" );
    print( "- Timeout: Enter sleep mode" );
}

void AfterMatchWinState::_stop_the_clock( PickleListenerContext& context ) {
    if ( context.getClockTimer() != nullptr && context.getClockTimer()->isRunning()) {
        context.getClockTimer()->reset();
        print( "*** ClockTimer stopped for AFTER_MATCH_WIN_STATE ***" );
    }

    if ( context.getClockUpdater() != nullptr && context.getClockUpdater()->isRunning()) {
        context.getClockUpdater()->stop(); // no need to reset anything, the updater just updates.
        print( "*** ClockUpdater thread stopped for AFTER_MATCH_WIN_STATE ***" );
    }
}
