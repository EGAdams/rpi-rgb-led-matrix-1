#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Blinker/Blinker.h"
#include "../KeyboardInputWithTimer/KeyboardInputWithTimer.h"

KeyboardInputWithTimer::KeyboardInputWithTimer( Blinker* blinker, unsigned long timeout_ms )
    : IInputWithTimer( blinker, timeout_ms ), _elapsedTimeMs( 0 ) {
}  // initialize elapsedTimeMs to 0

KeyboardInputWithTimer::~KeyboardInputWithTimer() {
    print( "\n\n*** Restoring terminal state. *** \n\n" );
    struct termios oldt;
    tcgetattr( 0, &oldt );
    _restoreTerminal( oldt, fcntl( 0, F_GETFL, 0 ) );
}

bool KeyboardInputWithTimer::validateInput( int selection ) const {
    return  selection == GREEN_REMOTE_GREEN_SCORE   ||
            selection == GREEN_REMOTE_RED_SCORE     ||
            selection == RED_REMOTE_GREEN_SCORE     ||
            selection == RED_REMOTE_RED_SCORE       ||
            selection == UMPIRE_REMOTE_GREEN_SCORE  ||
            selection == UMPIRE_REMOTE_RED_SCORE    ||
            selection == GREEN_REMOTE_UNDO          ||
            selection == RED_REMOTE_UNDO            ||
            selection == UMPIRE_REMOTE_UNDO;
}

int KeyboardInputWithTimer::_configureTerminal( struct termios& oldt ) {
    struct termios newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO ); // Disable canonical mode and echo
    tcsetattr( 0, TCSANOW, &newt );

    int old_flags = fcntl( 0, F_GETFL, 0 );
    fcntl( 0, F_SETFL, old_flags | O_NONBLOCK );
    return old_flags;
}

void KeyboardInputWithTimer::_restoreTerminal( const struct termios& oldt, int old_flags ) {
    tcsetattr( 0, TCSANOW, &oldt );
    fcntl( 0, F_SETFL, old_flags );
}


int KeyboardInputWithTimer::getInput() {
    using namespace std::chrono;

    struct termios oldt;
    tcgetattr( 0, &oldt );
    int old_flags = _configureTerminal( oldt );

    _startTime = steady_clock::now();
    _elapsedTimeMs = 0;
    int selection = 0;
    std::string inputBuffer;

    if (!_blinker) {
        print("*** ERROR: _blinker is NULL in RemoteInputWithTimer::getInput() ***");
        return -1; // Return error instead of crashing
    }
    // print("*** _blinker address: " + std::to_string(reinterpret_cast<uintptr_t>(_blinker.get())) + " ***");
    // print("*** RemoteInputWithTimer::getInput() - calling _blinker->start() ***");
    _blinker->start();  // This was causing the segmentation fault before

    try {
        while ( true ) {
            auto now = steady_clock::now();
            _elapsedTimeMs = duration_cast< milliseconds >( now - _startTime ).count();

            if ( _elapsedTimeMs >= _timeout_ms ) {
                print( "\n\n\n\n*** Timeout ***" );
                print( "Keyboard input timed out after " << _timeout_ms / 1000 << " seconds. 012525" );
                print( "\n\n\n\n" );
                _restoreTerminal( oldt, old_flags );
                _blinker->stop();
                return INPUT_TIMEOUT_CODE;
            }

            char ch;
            while ( read( 0, &ch, 1 ) > 0 ) {
                if ( ch == '\n' ) {
                    try {
                        selection = std::stoi( inputBuffer );
                        if ( validateInput( selection ) ) {
                            print( "Valid input received: " << selection );
                            _restoreTerminal( oldt, old_flags );
                            _blinker->stop();
                            return selection;
                        } else {
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
                            inputBuffer.clear();
                        }
                    }
                    catch ( const std::invalid_argument& ) {
                        // print( "Invalid input. Please enter a number." );
                        inputBuffer.clear();
                    }
                }
                else {
                    inputBuffer += ch;
                }
            }
            std::this_thread::sleep_for( milliseconds( 50 ) ); // Reduce CPU usage
        }
    }
    catch ( ... ) {
        _restoreTerminal( oldt, old_flags );
        throw;
    }
    _restoreTerminal( oldt, old_flags );
    return -1;
}
