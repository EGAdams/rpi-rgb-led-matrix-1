#include "GAME_MODES.h"
#include "../Arduino/Arduino.h"    // Arduino.h includes iostream, string, and defines OUTPUT and LOW
#include "../GameState/GameState.h"

GameModes::~GameModes() {}
GameModes::GameModes( 
    Team* team_a,
    Team* team_b,
    PinInterface* pinInterface,
    Rules* rules, 
    ScoreBoard* scoreBoard ) :
    _team_a( team_a ),
    _team_b( team_b ),
    _pinInterface( pinInterface ),
    _rules( rules ),
    _scoreBoard( scoreBoard ),
    _inputs( team_a, team_b, pinInterface, scoreBoard ),
    _reset( pinInterface ) {
        _gameStateManager = new GameStateManager( scoreBoard );
        _remoteLocker     = new RemoteLocker(     _team_a    );
    }

void GameModes::undo( GameState* gameState, History& history ) {
    _gameStateManager->setPreviousGameState( gameState, _team_a, _team_b, history );
    gameState->setPlayerButton( 0 );
}

void GameModes::gameStart( GameState* gameState, History& /*history*/ ) {
    if ( gameState->getStarted()  == 0 ) {  // if not started...
        _team_a->setPoints(          0   ); 
        _team_b->setPoints(          0   ); 
        gameState->setCurrentSet(    1   );
        gameState->setTeamASets(     0   );
        gameState->setTeamBSets(     0   );
        _team_a->setSets(            0   ); 
        _team_b->setSets(            0   ); 
        _team_a->setSets( gameState, 0   ); 
        _team_b->setSets( gameState, 0   ); 
        _team_a->setServe(           1   ); // turn off during pairing // 061225  TODO: turn off for pairing
        _team_b->setServe(           0   );
        
        // FIXED: Don't call redundant ScoreBoard update that overwrites center divider
        // The ScoreBoard will be updated by the main game loop anyway
        std::cout << "[GAMESTART] *** SKIPPING redundant _scoreBoard->update() to preserve center divider! ***" << std::endl;
        // _scoreBoard->update();  // COMMENTED OUT: This was overwriting the center divider
        
        gameState->setStarted(       1   );
    } else {
        std::cout << "[GAMESTART] Game already started, skipping initialization." << std::endl;
    }
}

void GameModes::mode1( GameState* gameState, History& history ) {}

void GameModes::mode2( GameState* gameState, History& history ) {
    gameState->setNow( GameTimer::gameMillis()); // eatMoreCookie$in2022
    mode1( gameState, history ); }

void GameModes::mode4( GameState *gameState, History& history) {
    gameState->setNow( GameTimer::gameMillis() );
    mode1( gameState, history ); }

void GameModes::noCode() {
    _team_a->setPoints( _team_a->getPoints() + 1 );  // p1Points++;
    GameTimer::gameDelay( 1000 );
    _team_a->setPoints( _team_a->getPoints() - 1 );  // p1Points--;
    GameTimer::gameDelay( 1000 ); }

void GameModes::setGameMode( int rotaryPosition, GameState* gameState, History& history ) {
    std::cout << "[DEBUG] setGameMode() called with rotaryPosition=" << rotaryPosition 
              << ", gameState->getStarted()=" << gameState->getStarted() << std::endl;
    switch ( rotaryPosition ) {
    case 0: break;
    case SINGLES_MODE:
        _rules->setFreshServes( 1  );
        _rules->setPointsToWin( 11 );
        _rules->setMaxPoints(   MAX_POINTS );
        gameStart( gameState, history );  // sets gameStart to true. resets player and score board.
        mode1(     gameState, history );
        break;

    case DOUBLES_MODE:                      // 061225 all of this may not be needed
        _rules->setFreshServes( 2  );       // 061225 mode could be set in the state machine
        _rules->setPointsToWin( 11 );
        _rules->setMaxPoints(   MAX_POINTS );   
        gameStart( gameState, history );
        mode1(     gameState, history );    // 061325 this mode1 does nothing
        break;                              // 061325 it has pairing mode, but we need to put
                                            // 061325 into a state
    case 3:  // Game mode 3 (Not yet written)
      // Serial.println("Game Mode 3 redirecting to Mode1...");
        gameStart( gameState, history );
        mode1(     gameState, history );
        // Mode2();
        // noCode();
        break;

    case 4:  // Game mode 4 (Not yet written)
      // Serial.println("Game Mode 4");
        gameStart( gameState, history );
        mode4(     gameState, history );
        // Mode2();
        // noCode();
        break;

    case 5:  // Game mode 5 (Not yet written)
      // Serial.println("Game Mode 5");
      // Mode2();
        noCode();
        break;
    }
}
