Here is the original InputWithTimer.cpp file.  It may have bugs in it, I don't know.  I am trying to debug the whole system and breaking the code up into Interfaces as we have been doing is helping my thought process.  So please analyze the following code and rewrite this one too, just like you did with the last one.  Use any interfaces that we have already created.  Create more if you need to.  Write your solution all into one file including a main so that I can g++ it and run it.  Put it all in one code block so that I can do this without making mistakes.  Here is the code:
```cpp
#include "InputWithTimer.h"
#include <iostream>
#include "../TennisConstants/TennisConstants.h"

InputWithTimer::InputWithTimer( Blinker* blinker, Inputs* inputs ) : _blinker( blinker ), _inputs( inputs ) {
    _time_slept = 0;
}
InputWithTimer::~InputWithTimer() {}

int InputWithTimer::getInput() {
    unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time with game timer
    int selection;
    bool done = false;  
    print( "starting blinker from within InputWithTimer..." );
    _blinker->start();
    print( "getting input from within InputWithTimer..." );
    if ( REMOTE_INPUT == 1 ) {  // 122224
        while ( !done ) {                           // remote mode
            print( "*** reading selection from inputs... ***" ); // 122224
            selection = _inputs->read_mcp23017_value();
            std::cout << "read selection from inputs: " << selection << std::endl;
            if ( selection == GREEN_REMOTE_GREEN_SCORE  || 
                 selection == GREEN_REMOTE_RED_SCORE    ||
                 selection == RED_REMOTE_GREEN_SCORE    ||
                 selection == RED_REMOTE_RED_SCORE ) {
                std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                done = true;
            } else { 
                // delay 250ms
                std::cout << "sleeping 250ms..." << std::endl; 
                GameTimer::gameDelay( 250 );
            }
        }
    } else if ( REMOTE_INPUT == 0 ) {  // menu mode // 122224
        std::cout << "Enter your selection: ";
        std::cin >> selection;
        print( "made seleciton in InputWithTimer::getInput()... " );
        print( "selection: " << selection );

    } else {
        std::cout << "*** ERROR: Invalid input mode in InputWithTimer Object getInput() method. ***\n";
        exit( 1 );
    }
    _blinker->stop();
    unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time with game timer
    _time_slept = sleep_end - sleep_start;
    return selection;
}

unsigned long InputWithTimer::getTimeSlept() {
    return _time_slept;
}
```