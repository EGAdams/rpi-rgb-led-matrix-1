#include "SleepModeState.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"

void SleepModeState::handleInput( PickleListenerContext& context ) {
    context.lock();                             // Lock to keep thread safety
    print( "==========================" );
    print( "=== [STATE: SleepMode] ===" );
    print( "==========================\n\n\n" );
    
    // Stop the clock timer when entering SLEEP_MODE_STATE
    if (context.getClockTimer() != nullptr) {
        context.getClockTimer()->stop();
        print( "*** ClockTimer stopped for SLEEP_MODE_STATE ***" );
    }
    int selection = context.getSleepingInputWithTimer()->getInput();    // Block for input from sleepingInputWithTimer    
    if ( selection == GREEN_REMOTE_GREEN_SCORE  ||                      // Check if the user pressed a valid score button 
         selection == GREEN_REMOTE_RED_SCORE    ||                      // during sleep
         selection == RED_REMOTE_GREEN_SCORE    ||
         selection == RED_REMOTE_RED_SCORE      ||
         selection == UMPIRE_REMOTE_GREEN_SCORE ||
         selection == UMPIRE_REMOTE_RED_SCORE   ||
         selection == RED_REMOTE_UNDO           ||
         selection == GREEN_REMOTE_UNDO         ||
         selection == UMPIRE_REMOTE_UNDO        
        ) {
        print( "Time slept: " + std::to_string( context.getSleepingInputWithTimer()->getTimeSlept()));
        
        print( "if we are sleeping because of no remote activity, then we should wake up in regular play without adjusting the score." );
        if ( context.getGameState()->getGamePlayState() == REGULAR_PLAY_SLEEP_STATE ) {
            // Start the clock timer when waking up and transitioning to REGULAR_PLAY_AFTER_SCORE_STATE
            if (context.getClockTimer() != nullptr) {
                context.getClockTimer()->start();
                print( "*** ClockTimer started for wake up transition to REGULAR_PLAY_AFTER_SCORE_STATE ***" );
                
                // Set the ClockTimer in the ScoreBoard so it can display elapsed time
                if (context.getScoreboard() != nullptr) {
                    context.getScoreboard()->setClockTimer(context.getClockTimer());
                    print( "*** ClockTimer set in ScoreBoard for elapsed time display (wake up) ***" );
                }
            }
            
            context.getGameState()->setGamePlayState( REGULAR_PLAY_AFTER_SCORE_STATE ); // Wake up in regular play
            context.unlock();                                                           // Unlock before returning
            return;             // <---------------- return -------------------<<
        }

        if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO || selection == UMPIRE_REMOTE_UNDO ) {
            print( "Undoing last action..." );
            context.getGameObject()->undo();
        } else {                                // must have it a score button, reset match...
            print( "not doing anything here except clearing off the bouncing ball and setting the state..." );
            print( "Resetting timers..." );
            _stop_the_clock( context );
            print( "timer reset complete." );
        }
        print( "Clearing scoreboard screen..." );                       // clean off bouncing ball
        context.getScoreboard()->clearScreen();
        print( "Screen cleared." );
        // context.getScoreboard()->update();                           // Shows Zeros!
        // print( "Scoreboard updated." );
        context.getGameState()->setCurrentAction( AFTER_SLEEP_MODE );           // Transition to AFTER_SLEEP_MODE
        if ( context.getGameState()->getGamePlayState() == NO_SCORE_SLEEP_STATE ) {     
            context.getGameState()->setGamePlayState( REGULAR_PLAY_NO_SCORE_STATE );    // Wake up in the beginning of the game
        } else if ( context.getGameState()->getGamePlayState() == PAIRING_SLEEP_MODE_STATE ) {
            context.getGameState()->setGamePlayState( PAIRING_MODE_STATE ); // Wake up in pairing mode
        }
        context.unlock();  // Unlock before returning
        return;
    }
    print( "*** Warning: unknown input in Sleep Mode.  no action taken except unlocking context... ***" );
    context.unlock();  // Unlock before returning
}

void SleepModeState::_stop_the_clock( PickleListenerContext& context ) {
    if ( context.getClockTimer() != nullptr && context.getClockTimer()->isRunning()) {
        context.getClockTimer()->reset();
        print( "*** ClockTimer stopped for AFTER_MATCH_WIN_STATE ***" );
    }

    // Stop the clock updater thread when entering AFTER_MATCH_WIN_STATE (if not already stopped)
    if ( context.getClockUpdater() != nullptr && context.getClockUpdater()->isRunning()) {
        context.getClockUpdater()->stop(); // no need to reset anything, the updater just updates.
        print( "*** ClockUpdater thread stopped for AFTER_MATCH_WIN_STATE ***" );
    }
}