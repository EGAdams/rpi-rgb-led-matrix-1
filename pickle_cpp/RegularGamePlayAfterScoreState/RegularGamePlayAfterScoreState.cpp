#include "RegularGamePlayAfterScoreState.h"
#include "../GameStateManager/GameStateManager.h"

void RegularGamePlayAfterScoreState::handleInput( PickleListenerContext& context ) {
    context.lock();  // Ensure thread safety

    print( "===============================================" );
    print( "=== [STATE: RegularGamePlayAfterScoreState] ===" );
    print( "===============================================\n\n\n" );
    
    // Start the clock timer when entering REGULAR_PLAY_AFTER_SCORE_STATE (first time only)
    if (context.getClockTimer() != nullptr && !context.getClockTimer()->isRunning()) {
        context.getClockTimer()->start();
        print( "*** ClockTimer started for REGULAR_PLAY_AFTER_SCORE_STATE ***" );
        
        // Set the ClockTimer in the ScoreBoard so it can display elapsed time
        if (context.getScoreboard() != nullptr) {
            context.getScoreboard()->setClockTimer(context.getClockTimer());
            print( "*** ClockTimer set in ScoreBoard for elapsed time display ***" );
        }
    }
    
    // Start the clock updater thread for regular display updates (first time only)
    if (context.getClockUpdater() != nullptr && !context.getClockUpdater()->isRunning()) {
        // Set the ScoreBoard so the ClockUpdater can trigger updates
        context.getClockUpdater()->setScoreBoard(context.getScoreboard());
        context.getClockUpdater()->start();
        print( "*** ClockUpdater thread started for regular display updates ***" );
    }
    context.getScoreboard()->update();
    context.getNoBlinkInputWithTimer()->setTimeout( REGULAR_GAME_PLAY_INPUT_TIMEOUT );
    int button = context.getNoBlinkInputWithTimer()->getInput(); // Use noBlinkInputWithTimer to detect inactivity    
    if ( button != 15 ) { print( "Selection from gameInput: \n\n*** " + TranslateConstant().get_translated_constant( button ) + " ***\n" );}
    if ( button == 0 ) {  context.unlock(); return; }
    if ( button == INPUT_TIMEOUT_CODE ) {
        print( "\n\n***********************************************************" );
        print( "*** No Remotes Detected Timeout! Going to sleep mode... ***"     );
        print( "***********************************************************\n\n" );
        
        // Stop the clock timer when leaving REGULAR_PLAY_AFTER_SCORE_STATE
        if (context.getClockTimer() != nullptr) {
            context.getClockTimer()->stop();
            print( "*** ClockTimer stopped for state transition ***" );
        }
        
        // Stop the clock updater thread when leaving REGULAR_PLAY_AFTER_SCORE_STATE
        if (context.getClockUpdater() != nullptr) {
            context.getClockUpdater()->stop();
            print( "*** ClockUpdater thread stopped for state transition ***" );
        }
        
        context.getGameState()->setCurrentAction( SLEEP_MODE                     );
        context.getGameState()->setGamePlayState( REGULAR_PLAY_SLEEP_STATE       );
        context.getGameObject()->getScoreBoard()->clearScreen();    // clear scoreboard for
        context.unlock();                                           // bliniing ball show
        return;
    }

    // Check RemoteLocker before processing any score commands
    if ( context.getRemoteLocker()->playerNotServing( button )) {
        std::cout << "*** Remote locked out - player not serving ***" << std::endl;
        context.unlock();
        return;
    }

    if ( button == RED_REMOTE_GREEN_SCORE || button == GREEN_REMOTE_GREEN_SCORE ) { // GREEN Score
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
    }
    
    if ( button == RED_REMOTE_UNDO || button == GREEN_REMOTE_UNDO ) {  // UNDO
        std::cout << "*** DEBUG: Processing UNDO command ***" << std::endl;
        
        // Create GameStateManager locally like other components do
        GameStateManager* gameStateManager = new GameStateManager( context.getGameObject()->getScoreBoard());
        
        // Access teams and history through context/GameObject
        Team* team_a = context.getGameObject()->getTeamA();
        Team* team_b = context.getGameObject()->getTeamB();
        std::cout << "*** DEBUG: Getting history shared_ptr from GameObject ***" << std::endl;
        auto history = context.getGameObject()->history();
        std::cout << "*** DEBUG: Retrieved history shared_ptr: " << history.get() 
                  << " (use_count: " << history.use_count() << ")" << std::endl;
        
        std::cout << "*** DEBUG: Created GameStateManager and retrieved objects from context ***" << std::endl;
        std::cout << "*** DEBUG: About to call setPreviousGameState ***" << std::endl;
        std::cout << "*** DEBUG: History object at address: " << history.get() 
                  << " (use_count: " << history.use_count() << ")" << std::endl;
        
        gameStateManager->setPreviousGameState( context.getGameState(), team_a, team_b, *history );
        
        std::cout << "*** DEBUG: setPreviousGameState completed successfully ***" << std::endl;
        context.getGameState()->setPlayerButton( 0 );

        context.getGameObject()->getScoreBoard()->update();  // dont forget the update after the game state change.
        
        // Clean up the locally created GameStateManager
        delete gameStateManager;
    }
    

    if ( button == RED_REMOTE_RED_SCORE || button == GREEN_REMOTE_RED_SCORE ) {  // RED Score
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

    if ( button == RED_REMOTE_RESET || button == GREEN_REMOTE_RESET ) {  // RESET
        std::cout << "*** DEBUG: Processing RESET command ***" << std::endl;
        
        // Access Reset object through context/GameObject
        Reset* reset = context.getGameObject()->getReset();
        
        // Create GameStateManager locally for saving game state
        GameStateManager* gameStateManager = new GameStateManager( context.getGameObject()->getScoreBoard() );
        
        // Access teams and history through context/GameObject
        Team* team_a = context.getGameObject()->getTeamA();
        Team* team_b = context.getGameObject()->getTeamB();
        auto history = context.getGameObject()->history();
        
        std::cout << "*** DEBUG: Retrieved objects from context ***" << std::endl;
        std::cout << "*** DEBUG: About to call resetScoreboard ***" << std::endl;
        
        reset->resetScoreboard( context.getGameState() );
        
        std::cout << "*** DEBUG: About to call saveGameState ***" << std::endl;
        gameStateManager->saveGameState( context.getGameState(), team_a, team_b, *history );
        
        std::cout << "*** DEBUG: RESET operations completed successfully ***" << std::endl;
        context.getGameState()->setPlayerButton( 0 );
        
        // Clean up the locally created GameStateManager
        delete gameStateManager;
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( SCORE_DELAY_IN_MILLISECONDS ));
    std::cout << "after score delay in milliseconds in Regular Game Play After Score State." << std::endl;
    context.getGameState()->setPlayerButton( button );
    context.unlock();
}

void RegularGamePlayAfterScoreState::showHelp() {
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
