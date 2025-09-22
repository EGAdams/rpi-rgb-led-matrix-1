#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <fstream>
#include <unordered_map>
#include "RemoteInputWithTimer.h"
#include "../TennisConstants/TennisConstants.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../PairingBlinker/PairingBlinker.h"

RemoteInputWithTimer::RemoteInputWithTimer(Blinker* blinker, Inputs* inputs, unsigned long timeout_ms)
    : IInputWithTimer(blinker, timeout_ms), _blinker(blinker), _inputs(inputs) {
    
    print( "DEBUG: RemoteInputWithTimer constructor called with blinker = " 
          << blinker << " and inputs = " << inputs );


    print( "DEBUG: RemoteInputWithTimer constructor called, this = " << this );

    if (!_blinker) {
        print("*** ERROR: RemoteInputWithTimer received a NULL blinker! ***");
    } else {
        print("*** RemoteInputWithTimer initialized with valid blinker at address: " +
              std::to_string(reinterpret_cast<uintptr_t>( _blinker )) + " ***");
    }

    // Initialize the input map with the defines from constants (copied from TestInputWithTimer)
    _input_map = {
        { "RED_REMOTE_RED_SCORE",       6  },
        { "RED_REMOTE_GREEN_SCORE",     10 },
        { "RED_REMOTE_UNDO",            2  },
        { "RED_REMOTE_RESET",           4  },
        { "GREEN_REMOTE_GREEN_SCORE",   7  },
        { "GREEN_REMOTE_RED_SCORE",     11 },
        { "GREEN_REMOTE_UNDO",          3  },
        { "GREEN_REMOTE_RESET",         8  },
        { "UMPIRE_REMOTE_GREEN_SCORE",  5  },
        { "UMPIRE_REMOTE_RED_SCORE",    9  },
        { "UMPIRE_REMOTE_UNDO",         1  },
        { "NO_REMOTE_BUTTONS",          0  },
        { "UNKNOWN_REMOTE_BUTTON",      15 }
    };
}
RemoteInputWithTimer::~RemoteInputWithTimer() { print("*** RemoteInputWithTimer destructor called ***"); }

int RemoteInputWithTimer::getInput() {
    using namespace std::chrono;
    auto startTime = steady_clock::now();
    unsigned long elapsedTimeMs = 0;
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;
    if (!_blinker) {
        print("*** ERROR: _blinker is NULL in RemoteInputWithTimer::getInput() ***");
        return -1; // Return error instead of crashing
    }
    // print("*** _blinker address: " + std::to_string(reinterpret_cast<uintptr_t>(_blinker.get())) + " ***");
    // print("*** RemoteInputWithTimer::getInput() - calling _blinker->start() ***");
    _blinker->start();  // This was causing the segmentation fault before
    print( "getting input from within RemoteInputWithTimer..." );
    print( "*** DEBUG: REMOTE_INPUT value = " << REMOTE_INPUT );
    if ( REMOTE_INPUT == 1 ) {
        print( "*** DEBUG: Entering REMOTE_INPUT==1 branch (hardware remote input mode)" ); 
        /*// if the selection is never one of the valid remote inputs, then we will never exit the while loop! // 011925
         * there is no timer here, the agent says.  in the future we will take out this comment and ask a new employee a question about this dillemma.  we will ask them to explain why this is a bad design because it locks up the system. meaning,  so how do we fix this?
         * the timer that would allow us to break out of this while loop is not in the scope of this while loop! we need either to make a timer in here, or pass in a timer object to this function.  */
        while ( !done ) {
            auto now = steady_clock::now();
            elapsedTimeMs = duration_cast< milliseconds >( now - startTime ).count();

            // Check if blinker has been stopped externally (e.g., by PairingBlinker timeout)
            // Cast to PairingBlinker to access awake() method
            PairingBlinker* pairingBlinker = dynamic_cast<PairingBlinker*>(_blinker);
            if ( pairingBlinker && !pairingBlinker->awake() ) {
                print( "******** Blinker stopped externally - exiting input loop ********" );
                return INPUT_TIMEOUT_CODE;
            }

            // For sleep mode (SLEEP_FOREVER), prioritize button processing over timeout
            if ( _timeout_ms != SLEEP_FOREVER && elapsedTimeMs >= _timeout_ms ) {
                print( "******** Input timed out after " << _timeout_ms / 1000 << " seconds. ********" );
                print( "stopping the blinker!" );
                _blinker->stop();
                return INPUT_TIMEOUT_CODE; // <--<< this is where we break out of the loop!
            } else if ( _timeout_ms != SLEEP_FOREVER ) {
                print( "elapsed time in milliseconds: " << elapsedTimeMs );
                print( "_timeout_ms:                  " << _timeout_ms   );
            }
            selection = _inputs->read_mcp23017_value();             // this actually does have a while. // 011925
            
            // Only show detailed debug info when we get a non-idle value
            if ( selection != UNKNOWN_REMOTE_BUTTON ) {}
            
            if ( selection == GREEN_REMOTE_GREEN_SCORE  ||           // isnt as bad as this
                 selection == GREEN_REMOTE_RED_SCORE    ||           // one because it always returns a value // 011925
                 selection == UMPIRE_REMOTE_GREEN_SCORE ||
                 selection == RED_REMOTE_GREEN_SCORE    ||          
                 selection == RED_REMOTE_RED_SCORE      ||
                 selection == UMPIRE_REMOTE_RED_SCORE   ||
                 selection == GREEN_REMOTE_UNDO         ||
                 selection == RED_REMOTE_UNDO           ||
                 selection == UMPIRE_REMOTE_UNDO        
            ) {
                print( "*** MATCH FOUND! selection: " << selection << " triggered the done flag, exiting while loop..." );
                done = true;
            } else {
                int delay = 3; // ms                             // TODO: check this one for power use
                if ( selection != UNKNOWN_REMOTE_BUTTON ) {
                    print( "*** NO MATCH: selection " << selection << " does not match any remote constant, sleeping " << delay << "ms..." );
                }
                GameTimer::gameDelay( delay );
            }
        }
    } else if ( REMOTE_INPUT == 0 ) {
        print( "Enter your selection: " );
        std::cin >> selection;
    } else {
        print( "*** ERROR: Invalid input mode in RemoteInputWithTimer Object getInput() method. ***\n" );
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
}
