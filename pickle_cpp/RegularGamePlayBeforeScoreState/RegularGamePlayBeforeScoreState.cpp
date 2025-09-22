/*
 * Zero score timeout state
 */
#include "RegularGamePlayBeforeScoreState.h"

void RegularGamePlayBeforeScoreState::handleInput( PickleListenerContext& context ) {
    context.lock();  // Ensure thread safety
    print( "================================================" );
    print( "=== [STATE: RegularGamePlayBeforeScoreState] ===" );
    print( "================================================\n\n\n" );
    
    context.getScoreboard()->update();
    if (context.getClockTimer() != nullptr) { // reset the clock timer
        context.getClockTimer()->reset();     // when entering REGULAR_PLAY_BEFORE_SCORE_STATE
        print( "*** ClockTimer stopped for REGULAR_PLAY_BEFORE_SCORE_STATE ***" );
    }

    // stop the clock updater thread when entering REGULAR_PLAY_BEFORE_SCORE_STATE
    if ( context.getClockUpdater() != nullptr && context.getClockUpdater()->isRunning()) {
        context.getClockUpdater()->stop();
        print( "*** ClockUpdater thread stopped for REGULAR_PLAY_BEFORE_SCORE_STATE ***" );
    }
    
    context.getScoreboard()->update(); // update the scoreboard to show 00:00 timer
    int selection = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity
    print( "Selection from noBlinkInputWithTimer: " + std::to_string( selection ));

    if ( selection == INPUT_TIMEOUT_CODE ) {
        print( "*** Zero Score Timeout! Going to sleep mode... ***"     );
        context.getGameState()->setCurrentAction( SLEEP_MODE            );
        context.getGameState()->setState( NO_SCORE_SLEEP_STATE          );
        context.getGameState()->setGamePlayState( NO_SCORE_SLEEP_STATE  );
        context.unlock();
        return;
    }

    bool& no_score_flag = context.getNoScoreFlag(); // We've received a valid input, so the 
    no_score_flag = false;                          // "no score" condition is no longer valid
    handleSelectionAndUpdate( context, selection ); // Handle the selection and update game state accordingly
    
    context.getGameState()->setState( REGULAR_PLAY_AFTER_SCORE_STATE ); // After a valid score input,
    context.getGameState()->setGamePlayState( REGULAR_PLAY_AFTER_SCORE_STATE );
    context.unlock();                                                   // transition to REGULAR_PLAY_AFTER_SCORE
}

void RegularGamePlayBeforeScoreState::handleSelectionAndUpdate( PickleListenerContext& context, int selection ) {
    if ( selection == 0 ) {
        print( "*** Invalid selection ( 0 )! ***" );
        return;
    }

    // Check if the correct player is serving
    int serve_flag = context.getRemoteLocker()->playerNotServing( selection );
    print( "*** serve_flag: " + std::to_string( serve_flag ) + " ***" );
    print( "*** before checking score flag in RegularGamePlayBeforeScoreState ***" );
    if ( serve_flag ) {
        print( "*** Warning: player not serving! ***" );
        return;
    }

    // If a valid scoring button is pressed
    if ( selection == GREEN_REMOTE_GREEN_SCORE  ||
         selection == GREEN_REMOTE_RED_SCORE    ||
         selection == RED_REMOTE_GREEN_SCORE    ||
         selection == RED_REMOTE_RED_SCORE      ||
         selection == UMPIRE_REMOTE_GREEN_SCORE ||
         selection == UMPIRE_REMOTE_RED_SCORE
        ) {
        if ( selection == GREEN_REMOTE_GREEN_SCORE || selection == RED_REMOTE_GREEN_SCORE || selection == UMPIRE_REMOTE_GREEN_SCORE ) {
            print( "*** Green player scored ***" );
            // selection = 1; // Represent GREEN
            std::cout << "this is a green score.  building score command..." << std::endl;
            auto history = context.getGameObject()->history(); // Pin lifetime
            ScoreCommand* scoreCommand = new ScoreCommand( context.getGameState(), 
                                                        context.getGameObject()->getTeamA(), // get team a
                                                        context.getGameObject()->getRules(),
                                                        context.getGameObject()->getScoreBoard(),
                                                        *history);
            std::cout << "score command constructed.  executing score command... " << std::endl;
            scoreCommand->execute();
            std::cout << "done executing score command." << std::endl;
            context.getGameState()->setPlayerButton( 0 );
            std::cout << "after setting player button to 0" << std::endl;
        } else if ( selection == GREEN_REMOTE_RED_SCORE || selection == RED_REMOTE_RED_SCORE || selection == UMPIRE_REMOTE_RED_SCORE ) {
            print( "*** Red player scored ***" );
            // selection = 2; // Represent RED
            std::cout << "this is a red score.  building score command..." << std::endl;
            auto history = context.getGameObject()->history(); // Pin lifetime
            ScoreCommand* scoreCommand = new ScoreCommand( context.getGameState(), 
                                                        context.getGameObject()->getTeamB(), // get team b
                                                        context.getGameObject()->getRules(),
                                                        context.getGameObject()->getScoreBoard(),
                                                        *history);

            std::cout << "score command constructed.  executing score command... " << std::endl;
            scoreCommand->execute();
            std::cout << "done executing score command." << std::endl;
            context.getGameState()->setPlayerButton( 0 );
            std::cout << "after setting player button to 0" << std::endl;
        }
        context.getGameObject()->playerScore( selection );
        print( "after setting player score in game object." );
    } else if ( selection == GREEN_REMOTE_UNDO || selection == RED_REMOTE_UNDO || selection == UMPIRE_REMOTE_UNDO ) {
        print( "*** Undo ***" );
        print( "before calling undo in RegularGamePlayBeforeScoreState" );
        context.getGameObject()->undo();
        print( "after calling undo in RegularGamePlayBeforeScoreState" );
    } else {
        print( "*** Invalid selection ***" );
        showHelp();
    }

    // Let the GameObject handle updates
    std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ));
    context.getGameObject()->loopGame();
}

void RegularGamePlayBeforeScoreState::showHelp() {
    print( "------------" );
    print( "GREEN REMOTE: " );
    print( "   green remote green score: "  <<  GREEN_REMOTE_GREEN_SCORE );
    print( "or green remote, red score: "   << GREEN_REMOTE_RED_SCORE );
    print( "or green remote, undo: "        << GREEN_REMOTE_UNDO );
    print( " ------------ \n");
    print( "RED REMOTE: " );
    print( "or red remote, green score: "   << RED_REMOTE_GREEN_SCORE );
    print( "or red remote, red score: "     << RED_REMOTE_RED_SCORE );
    print( "or red remote, undo: "          << RED_REMOTE_UNDO );
    print( " ------------ \n");
    print( "UMPIRE REMOTE: " );
    print( "or umpire remote, green score: " << UMPIRE_REMOTE_GREEN_SCORE );
    print( "or umpire remote, red score: "   << UMPIRE_REMOTE_RED_SCORE   );
    print( "or umpire remote, undo: "        << UMPIRE_REMOTE_UNDO );
    print( " ------------ \n");
}
