/************************************************************
 *  Example Refactoring of InputWithTimer.cpp to 
 *  "Program to Interfaces" in a Single Translation Unit
 *
 *  You can place ALL of this into a single .cpp file and
 *  compile/run it with something like:
 *      g++ -std=c++17 single_file.cpp -o program && ./program
 *
 *  This includes:
 *    - The interfaces (IInputs, IBlinker, IInputWithTimer).
 *    - The stubs/implementations (Blinker, Inputs, GameTimer, etc.).
 *    - The refactored InputWithTimer using those interfaces.
 *    - A main() to test it.
 *
 *  It's a simplified illustration to show how you might
 *  restructure your original InputWithTimer.cpp.
 ************************************************************/

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>    // for exit()
#include <atomic>     // if we need thread-safe counters, etc.

/*==========================================================
 *  Simple global "print" function (preserves style)
 *==========================================================*/
static void print( const std::string &msg ) {
    std::cout << msg << std::endl;
}

/*==========================================================
 *  Tennis Constants (like TennisConstants.h)
 *==========================================================*/
static const int GREEN_REMOTE_GREEN_SCORE = 1;
static const int GREEN_REMOTE_RED_SCORE   = 2;
static const int RED_REMOTE_GREEN_SCORE   = 3;
static const int RED_REMOTE_RED_SCORE     = 4;
static const int GREEN_REMOTE_UNDO        = 5;
static const int RED_REMOTE_UNDO          = 6;

// For demonstration, set REMOTE_INPUT to 0 or 1
// 0 => local "menu mode" input (std::cin)
// 1 => remote "read_mcp23017_value" input
static const int REMOTE_INPUT = 0;

/*==========================================================
 *  GameTimer: minimal reimplementation with gameMillis()
 *==========================================================*/
class GameTimer {
public:
    // Return current time in milliseconds since some epoch
    static unsigned long gameMillis() {
        using namespace std::chrono;
        auto now = system_clock::now().time_since_epoch();
        return static_cast<unsigned long>(
            duration_cast<milliseconds>( now ).count()
        );
    }

    // Sleep or delay for the specified number of milliseconds
    static void gameDelay( int ms ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
    }
};

/*==========================================================
 *  IInputs Interface
 *==========================================================*/
class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0;
};

/*==========================================================
 *  IBlinker Interface
 *  (We can reuse or unify PairingBlinker, ScoreboardBlinker, etc.
 *   For the example, we just define the minimal methods needed.)
 *==========================================================*/
class IBlinker {
public:
    virtual ~IBlinker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

/*==========================================================
 *  IInputWithTimer Interface
 *==========================================================*/
class IInputWithTimer {
public:
    virtual ~IInputWithTimer() = default;
    virtual int getInput() = 0;
    virtual unsigned long getTimeSlept() = 0;
};

/*==========================================================
 *  Concrete Blinkers
 *==========================================================*/
class Blinker : public IBlinker {
public:
    void start() override {
        print( "Blinker: start()" );
        // Real blinking logic would go here
    }
    void stop() override {
        print( "Blinker: stop()" );
        // Real logic to stop blinking
    }
};

/*==========================================================
 *  Concrete Inputs (simple stub)
 *==========================================================*/
class Inputs : public IInputs {
public:
    int read_mcp23017_value() override {
        // In real code, this would read the hardware input.
        // We'll return a pseudo-random "score" or "0" for illustration.
        static int calls = 0;
        calls++;
        // Just cycle through some valid/invalid values for demonstration:
        switch( calls % 5 ) {
            case 0: return GREEN_REMOTE_GREEN_SCORE;
            case 1: return GREEN_REMOTE_RED_SCORE;
            case 2: return RED_REMOTE_GREEN_SCORE;
            case 3: return RED_REMOTE_RED_SCORE;
            default: return 99; // invalid
        }
    }
};

/*==========================================================
 *  Refactored InputWithTimer Class
 *  (Implements the IInputWithTimer interface, uses IBlinker
 *   and IInputs interfaces.)
 *==========================================================*/
class InputWithTimer : public IInputWithTimer {
public:
    InputWithTimer( IBlinker* blinker, IInputs* inputs )
        : _blinker( blinker ), _inputs( inputs ), _time_slept( 0 ) 
    { 
    }

    ~InputWithTimer() override {}

    int getInput() override {
        unsigned long sleep_start = GameTimer::gameMillis(); // Mark start time
        int selection = 0;
        bool done = false;

        print( "starting blinker from within InputWithTimer..." );
        _blinker->start();

        print( "getting input from within InputWithTimer..." );
        if ( REMOTE_INPUT == 1 ) {  // remote mode
            while ( !done ) {
                print( "*** reading selection from inputs... ***" );
                selection = _inputs->read_mcp23017_value();
                std::cout << "read selection from inputs: " << selection << std::endl;
                if ( selection == GREEN_REMOTE_GREEN_SCORE  || 
                     selection == GREEN_REMOTE_RED_SCORE    ||
                     selection == RED_REMOTE_GREEN_SCORE    ||
                     selection == RED_REMOTE_RED_SCORE ) {
                    std::cout << "selection: " << selection << " triggered the done flag, exiting while loop..." << std::endl;
                    done = true;
                } else {
                    std::cout << "sleeping 250ms..." << std::endl; 
                    GameTimer::gameDelay( 250 );
                }
            }
        } else if ( REMOTE_INPUT == 0 ) {  // menu mode
            std::cout << "Enter your selection: ";
            std::cin >> selection;
            print( "made selection in InputWithTimer::getInput()... " );
            print( "selection: " + std::to_string( selection ) );
        } else {
            std::cout << "*** ERROR: Invalid input mode in InputWithTimer Object getInput() method. ***\n";
            exit( 1 );
        }

        _blinker->stop();

        unsigned long sleep_end = GameTimer::gameMillis(); // Mark end time
        _time_slept = sleep_end - sleep_start;
        return selection;
    }

    unsigned long getTimeSlept() override {
        return _time_slept;
    }

private:
    IBlinker*       _blinker;
    IInputs*        _inputs;
    unsigned long   _time_slept;
};

/*==========================================================
 *  Example main() for demonstration/testing
 *==========================================================*/
int main() {
    // Create our stub interfaces
    Blinker myBlinker;
    Inputs myInputs;

    // Create our InputWithTimer object (programming to interfaces)
    InputWithTimer inputWithTimer( &myBlinker, &myInputs );

    // Example usage
    int result = inputWithTimer.getInput();
    std::cout << "Result of getInput(): " << result << std::endl;
    std::cout << "Time slept (ms)     : " << inputWithTimer.getTimeSlept() << std::endl;

    return 0;
}
