#include "ClockUpdater.h"
#include "../ScoreBoard/ScoreBoard.h"

ClockUpdater::ClockUpdater( ClockTimer* clockTimer, ScoreBoard* scoreboard )
    : _clockTimer( clockTimer ), _scoreBoard( scoreboard ), _shouldStop( false ), _isRunning( false ), _updateThread() {
    std::cout << "ClockUpdater constructor called" << std::endl;
    std::cout << "ClockUpdater constructor completed successfully" << std::endl;
}

ClockUpdater::~ClockUpdater() { std::cout << "ClockUpdater destructor called" << std::endl; stop(); }

void ClockUpdater::start() {
    if ( _isRunning.load()) {      std::cout << "ClockUpdater already running" << std::endl;                    return; }
    if ( _clockTimer == nullptr ) { std::cout << "ClockUpdater: ClockTimer is null, cannot start" << std::endl; return; }
    _shouldStop.store( false );
    _isRunning.store(  true  );
    _updateThread = std::thread( &ClockUpdater::_updateLoop, this );
    std::cout << "ClockUpdater started" << std::endl;
}

void ClockUpdater::stop() { 
    if ( !_isRunning.load()) { return; }
    _shouldStop.store( true );
    if ( _updateThread.joinable()) { _updateThread.join(); }
    _isRunning.store( false );
    std::cout << "ClockUpdater stopped" << std::endl;
}

bool ClockUpdater::isRunning() const { return _isRunning.load(); }

void ClockUpdater::setScoreBoard( ScoreBoard* scoreBoard ) {
    _scoreBoard = scoreBoard;
    std::cout << "ClockUpdater: ScoreBoard set to " << scoreBoard << std::endl;
}

void ClockUpdater::_updateLoop() {
    std::cout << "ClockUpdater thread started" << std::endl;

    while ( !_shouldStop.load()) {
        std::this_thread::sleep_for( std::chrono::seconds( 1 ));  // Sleep for 1 second
        if ( _shouldStop.load()) { break; }

        // Trigger a scoreboard update to refresh the clock display
        if ( _scoreBoard != nullptr && _clockTimer != nullptr && _clockTimer->isRunning()) {
            if ( _scoreBoard->hasCanvas()) {
                _scoreBoard->update();
            } else {
                return;
            }
        }
    }
    std::cout << "ClockUpdater thread ended" << std::endl;
}
