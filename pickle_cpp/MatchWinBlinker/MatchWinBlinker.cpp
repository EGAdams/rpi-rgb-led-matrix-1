#include "MatchWinBlinker.h"
#define MATCH_WIN_OFFSET 12

MatchWinBlinker::MatchWinBlinker( ScoreBoard* scoreboard )
    : _scoreboard( scoreboard ), _shouldStop( false ), _running( false ) {
    // _scoreboard->setLittleDrawerFont( "the_sets_numbers.bdf" );
}

MatchWinBlinker::~MatchWinBlinker() { stop(); }

void MatchWinBlinker::start() {
    if ( _running.load() ) return;  // Prevent starting a new thread if already running

   
    if ( _blinkThread.joinable() ) { // If a previous thread finished but wasn't joined, join it. // 021725
        _blinkThread.join();         // https://chatgpt.com/share/67b38d3d-a5d4-8006-9f12-8c3e1e4bdb80
    }                                // this was causing a `terminate` early.  o3 nailed it right away.

    _shouldStop.store( false );
    _running.store( true );
    _blinkThread = std::thread( &MatchWinBlinker::blinkLoop, this );
}

void MatchWinBlinker::stop() {
    _shouldStop.store( true );
    if ( _blinkThread.joinable() ) { _blinkThread.join(); }
    _running.store( false );
}

bool MatchWinBlinker::isRunning() const { return _running.load(); }

void MatchWinBlinker::blinkLoop() {
    std::cout << "MatchWinBlinker started." << std::endl;
    for ( int blink_count = 0; blink_count < MATCH_WIN_BLINK_COUNT; blink_count++ ) {
        if ( _shouldStop.load() ) break;
        _scoreboard->clearScreen();
        _scoreboard->drawNewText( "Match", 13, 60 - MATCH_WIN_OFFSET );
        _scoreboard->drawNewText( "Win", 20, 75 - MATCH_WIN_OFFSET );
        // _scoreboard->drawSets();
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
        if ( _shouldStop.load() ) break;
        _scoreboard->clearScreen();
        GameTimer::gameDelay( MATCH_WIN_FLASH_DELAY );
    }
    std::cout << "MatchWinBlinker completed." << std::endl;
    _running.store( false );
}
