#include "MatchWinBlinkState.h"
#include "../PickleListenerContext/PickleListenerContext.h"
#include "../GameState/GameState.h"
#include "../GameObject/GameObject.h"
#include "../INPUTS/INPUTS.h"
#include "../History/History.h"
// #include "../TennisConstants/TennisConstants.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#define MATCH_WIN_OFFSET 12

MatchWinBlinkState::MatchWinBlinkState() {}

void MatchWinBlinkState::handleInput( PickleListenerContext& context ) {
    context.lock();
    print( "===================================" );
    print( "=== [STATE: MatchWinBlinkState] ===" );
    print( "===================================\n\n\n" );
    startBlinking( context );                                             // Start blinking asynchronously
    
    // start the timer for this thing
    _startTime = std::chrono::steady_clock::now();
    _elapsedTimeMs = 0;
    auto now = std::chrono::steady_clock::now();
    _elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>( now - _startTime ).count();
    unsigned long time_left = 0;

    unsigned long original_no_blink_time    = MAIN_INPUT_TIMEOUT;
    unsigned long match_blinking_time       = 100; // 2 * MATCH_WIN_FLASH_DELAY * MATCH_WIN_BLINK_COUNT + 100 /*ms*/; // change the timeout  
    context.getNoBlinkInputWithTimer()->setTimeout( match_blinking_time );  // of the no blink timer to the match
    while ( context.getMatchWinBlinker()->isRunning()) {                    // blinking time + 1 second for a buffer
        int selection = context.getNoBlinkInputWithTimer()->getInput();     // Listen for input while blinking
        _elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>( now - _startTime ).count();
        print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ));
        if ( selection == GREEN_REMOTE_GREEN_SCORE  ||
            selection == GREEN_REMOTE_RED_SCORE     ||
            selection == RED_REMOTE_GREEN_SCORE     ||
            selection == RED_REMOTE_RED_SCORE       ||
            selection == UMPIRE_REMOTE_GREEN_SCORE  ||
            selection == UMPIRE_REMOTE_RED_SCORE
        ) {
            print( "*** Player button pressed, ignore during Match Win Blink State... ***" );
            // context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
            // context.getScoreboard()->update();
            // context.getMatchWinBlinker()->stop();  // Stop blinking early
            // context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time );   // reset the main input 
            // context.unlock();                                                           // timeout of the no blink
            // return;
                                                                                // timer before returning.
            time_left = match_blinking_time - _elapsedTimeMs;                   // TODO: The next three lines of code may not
            print( "time left: " + std::to_string( time_left ));                // be needed.  timeout is set to 100ms
            context.getNoBlinkInputWithTimer()->setTimeout( time_left );        // effectively disabling it.
            continue;
        } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO ) {
            print( "*** Undo ***" );
            context.getGameObject()->undo();
            
            // Start the clock timer when transitioning to REGULAR_PLAY_AFTER_SCORE_STATE
            if (context.getClockTimer() != nullptr) {
                context.getClockTimer()->start();
                print( "*** ClockTimer started for match win undo transition to REGULAR_PLAY_AFTER_SCORE_STATE ***" );
                
                // Set the ClockTimer in the ScoreBoard so it can display elapsed time
                if (context.getScoreboard() != nullptr) {
                    context.getScoreboard()->setClockTimer(context.getClockTimer());
                    print( "*** ClockTimer set in ScoreBoard for elapsed time display (match win undo) ***" );
                }
            }
            
            context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE );
            context.getScoreboard()->update();
            context.getMatchWinBlinker()->stop(); // Stop blinking early
            context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time );   // reset the main input 
            context.unlock();                                                           // timeout of no blink
            return;                                                                     // timeout before returning.
        } else {
            print ( "unknown input during match blink.");
        }
    }
    print( "*** MatchWinBlinker finished blinking, transitioning to AfterMatchWinState ***" ); // No input. 
    context.getGameState()->setState( AFTER_MATCH_WIN_STATE );                                 // Blinkingcomplete.
    context.getScoreboard()->clearScreen();          // pressing score after breaking out of Match Win Blink State
    context.getScoreboard()->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawNewText( "Win", 20, 75 - MATCH_WIN_OFFSET );
    context.getScoreboard()->drawSets();
    context.getNoBlinkInputWithTimer()->setTimeout( original_no_blink_time ); // reset the main input blink time
    context.unlock();
}
void MatchWinBlinkState::startBlinking( PickleListenerContext& context ) {
    if ( !context.getMatchWinBlinker()->isRunning()) {
        context.getMatchWinBlinker()->start();
    }
}
