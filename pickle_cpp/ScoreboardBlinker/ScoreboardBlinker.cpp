#include "ScoreboardBlinker.h"
#include <chrono>
#include <thread>

ScoreboardBlinker::ScoreboardBlinker( ScoreBoard* scoreBoard ) :
    _scoreboard( scoreBoard ) {
        _sleep_start = GameTimer::gameMillis();;
    }

ScoreboardBlinker::~ScoreboardBlinker() { stop(); }

void ScoreboardBlinker::blinkLoop() {
    unsigned long time_now     = GameTimer::gameMillis();
    unsigned long time_elapsed = time_now - _sleep_start;
    while ( !should_stop ) {
        blinkTennisBall( true );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        time_now = GameTimer::gameMillis();
        time_elapsed = (time_now - _sleep_start) / 1000;
        print( "time_elapsed: " << time_elapsed );
        if ( time_elapsed >  MAX_SLEEP ) {
            print( "UNDO (9): Reset Game        Player1 Score (1): Reset Game  Player2 Score (2): Reset Game   slept " << time_elapsed << " seconds." );    
        }
        blinkTennisBall( false );
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        time_now = GameTimer::gameMillis();
        time_elapsed = ( time_now - _sleep_start ) / 1000;
        print( "time_elapsed: " << time_elapsed );
        if ( time_elapsed <= MAX_SLEEP ) {
            print( "UNDO (9): Enter last match  Player1 Score (1): Reset Game  Player2 Score (2): Reset Game   slept " << time_elapsed << " seconds.");    
        }
    }
}

void ScoreboardBlinker::blinkTennisBall( bool show ) {
    // Implement the logic to show/hide the tennis ball on the scoreboard
    // This will depend on your existing scoreboard implementation
    if ( show ) {
        // std::cout << "drawing yellow period... " << std::endl;
        _scoreboard->drawYellowPeriod();
    } else {
        _scoreboard->clearScreen();
    }
}

void ScoreboardBlinker::start() {
    should_stop = false;
    blink_thread = std::thread( &ScoreboardBlinker::blinkLoop, this );
}

void ScoreboardBlinker::stop() {
    should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

