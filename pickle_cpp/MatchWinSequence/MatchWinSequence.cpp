#include "MatchWinSequence.h"
#include <iostream>
#define MATCH_WIN_OFFSET 12
#define DRAW_BLANK_SETS 1
#define NORMAL_GAME_STATE 99
#define RUNNING_MATCH_WIN_SEQUENCE 1
#define ALL_SETS_FLASH_COUNT 5

MatchWinSequence::MatchWinSequence()  {}
MatchWinSequence::~MatchWinSequence() {}

void MatchWinSequence::run( GameState* gameState, ScoreBoard* scoreBoard ) {
    std::cout << "//////////////////////// MatchWinSequence::run() ////////////////////////" << std::endl;
    gameState->setCurrentAction( RUNNING_MATCH_WIN_SEQUENCE ); // so scoreboard knows...
    if ( scoreBoard->hasCanvas()) {
        scoreBoard->clearScreen();
        scoreBoard->setLittleDrawerFont( "fonts/8x13B.bdf" );
        print( "flash MATCH WIN... " );
        for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
            // scoreBoard->setLittleDrawerFont( "the_sets_numbers.bdf" );
            scoreBoard->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
            scoreBoard->drawNewText( "Win"  , 20, 75 - MATCH_WIN_OFFSET );
            scoreBoard->drawSets();
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            print( "done match win flash delay of " << MATCH_WIN_FLASH_DELAY );
            scoreBoard->drawNewText( "     ", 13, 60 - MATCH_WIN_OFFSET );
            scoreBoard->drawNewText( "   "  , 20, 75 - MATCH_WIN_OFFSET );
            // if ( blink_count == MATCH_WIN_BLINK_COUNT - 1 ) { scoreBoard->clearScreen(); }
            GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
            print( "done match win flash delay of " << MATCH_WIN_FLASH_DELAY );
        }

        scoreBoard->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET ); // 082625 draw match win
        scoreBoard->drawNewText( "Win"  , 20, 75 - MATCH_WIN_OFFSET ); // and sets back in so that
        scoreBoard->drawSets();                                        // we can freeze the screen.
        ClockTimer* clockTimer = scoreBoard->getClockTimer();
        if ( clockTimer != nullptr && clockTimer->isRunning()) { // if the clock timer is valid, reset it.
            clockTimer->reset();
            std::cout << "*** ClockTimer stopped immediately at match win ***" << std::endl;
        } else if (clockTimer == nullptr) {
            std::cout << "*** WARNING: ClockTimer is null in MatchWinSequence ***" << std::endl;
        } else {
            std::cout << "*** INFO: ClockTimer was not running when match won ***" << std::endl;
        }
    } else {
        // scoreBoard->clearScreen(); // 082625  not clearing screen for after match win state.
        gameState->setState( AFTER_MATCH_WIN_STATE );
        gameState->setGamePlayState( AFTER_MATCH_WIN_STATE );
        return;
    }

    // scoreBoard->clearScreen(); 
    gameState->setState( AFTER_MATCH_WIN_STATE );
    gameState->setGamePlayState( AFTER_MATCH_WIN_STATE );
}    
