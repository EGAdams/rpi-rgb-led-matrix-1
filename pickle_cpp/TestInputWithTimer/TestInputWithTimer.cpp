#include "TestInputWithTimer.h"
using namespace std::chrono;

TestInputWithTimer::~TestInputWithTimer() {
    std::cout << "destroying test input with timer..." << std::endl;
}

bool TestInputWithTimer::_onRaspberryPi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

TestInputWithTimer::TestInputWithTimer( Blinker* blinker, void* /*unused_inputs*/, unsigned long timeout_ms )
    : IInputWithTimer( blinker, timeout_ms ), _blinker( blinker ), _start_time( steady_clock::now() )
{
    // Initialize the input map with the defines from instructions
    _input_map = {
        { "RED_REMOTE_RED_SCORE",       6  },
        { "RED_REMOTE_GREEN_SCORE",     2  },
        { "RED_REMOTE_UNDO",            3  },
        { "RED_REMOTE_RESET",           4  },
        { "GREEN_REMOTE_GREEN_SCORE",   7  },
        { "GREEN_REMOTE_RED_SCORE",     1  },
        { "GREEN_REMOTE_UNDO",          5  },
        { "GREEN_REMOTE_RESET",         8  },
        { "UMPIRE_REMOTE_GREEN_SCORE",  5  },
        { "UMPIRE_REMOTE_RED_SCORE",    9  },
        { "UMPIRE_REMOTE_UNDO",         1  },
        { "NO_REMOTE_BUTTONS",          0  },
        { "UNKNOWN_REMOTE_BUTTON",      99 }
    };

    // Load test inputs from file
    if ( _onRaspberryPi()) {
        std::cout << "loading test inputs from: /home/dietpi/rpi-rgb-led-matrix/pickle_cpp/TestInputWithTimer/test_input.txt" << std::endl;
        _loadTestInputs( "/home/dietpi/rpi-rgb-led-matrix/pickle_cpp/TestInputWithTimer/test_input.txt" );
    } else {
        std::cout << "loading test inputs from: /home/adamsl/rpi-rgb-led-matrix/pickle_cpp/TestInputWithTimer/test_input.txt" << std::endl;
        _loadTestInputs( "/home/adamsl/rpi-rgb-led-matrix/pickle_cpp/TestInputWithTimer/test_input.txt" );
    }    
}

TestInputWithTimer::TestInputWithTimer( unsigned long timeout_ms )
    : TestInputWithTimer( nullptr, nullptr, timeout_ms ) {
        _start_time = steady_clock::now();
    }

void TestInputWithTimer::_loadTestInputs( const std::string& filename ) {
    std::ifstream infile( filename );
    if ( !infile.is_open()) {
        std::cerr << "*** ERROR: Failed to open test input file: " << filename << " exiting... ***" << std::endl;
        exit( 0 );
        return;
    }

    std::string line;
    while ( std::getline( infile, line )) {
        // std::cout << "line: " << line << std::endl;
        // Trim whitespace from line
        line.erase( line.begin(), std::find_if( line.begin(), line.end(), []( unsigned char ch ) {
            return !std::isspace( ch );
            } ));
        line.erase( std::find_if( line.rbegin(), line.rend(), []( unsigned char ch ) {
            return !std::isspace( ch );
            } ).base(), line.end());

        // Ignore empty lines and comment lines starting with '#'
        if ( line.empty() || line[0] == '#' ) {
            continue;
        }

        // Check if this is a PAUSE command
        if ( line.substr( 0, 5 ) == "PAUSE" ) {
            std::istringstream iss( line );
            std::string pause_cmd;
            int duration;
            if ( iss >> pause_cmd >> duration ) {
                TestCommand cmd;
                cmd.type = TestCommand::PAUSE;
                cmd.value = duration;
                _test_commands.push_back( cmd );
                std::cout << "Added pause command: " << duration << " seconds" << std::endl;
            } else {
                std::cerr << "Invalid PAUSE command format: " << line << std::endl;
            }
        } else {
            // Look up the line in _input_map
            auto it = _input_map.find( line );
            if ( it != _input_map.end()) {
                TestCommand cmd;
                cmd.type = TestCommand::INPUT;
                cmd.value = it->second;
                _test_commands.push_back( cmd );
            } else {
                std::cerr << "Unknown input in test file: " << line << std::endl;
                TestCommand cmd;
                cmd.type = TestCommand::INPUT;
                cmd.value = _input_map["UNKNOWN_REMOTE_BUTTON"];
                _test_commands.push_back( cmd );
            }
        }
        // std::cout << "test input size: " << _test_inputs.size() << std::endl;
    }
}

int TestInputWithTimer::getInput() {
    // auto startTime = steady_clock::now();
    
    if ( _blinker ) { _blinker->start(); }  // Start the blinker if available
    
    int result = 99;
    // Wait for input to be available or timeout
    while ( true ) {
        if ( _current_index < _test_commands.size()) {
            TestCommand& cmd = _test_commands[ _current_index++ ];
            
            if ( cmd.type == TestCommand::PAUSE ) {
                std::cout << "Pausing for " << cmd.value << " seconds..." << std::endl;
                std::this_thread::sleep_for( seconds( cmd.value ));
                std::cout << "Pause complete, continuing..." << std::endl;
                // Continue to next command
                continue;
            } else {
                // Regular input command
                result = cmd.value;
                break;
            }
        } else {
            // std::cout << "current index is greater than test command size, resetting current index... " << std::endl;
            // _current_index = 0;
            // result = _test_commands[ _current_index ];
            break;
        }
        auto now = steady_clock::now();
        unsigned long elapsedTimeMs = duration_cast< milliseconds >( now - _start_time ).count();
        if ( elapsedTimeMs >= _timeout_ms ) {
            std::cout << "******** Test input timed out after " << _timeout_ms / 1000 << " seconds. ********" << std::endl;
            result = INPUT_TIMEOUT_CODE;
            break;
        }
        std::this_thread::sleep_for( milliseconds( 10 )); // Sleep briefly to avoid busy waiting
    }

    std::cout << "exited while.  stopping blinker... " << std::endl;
    if ( _blinker ) {
        _blinker->stop();
        std::cout << "blinker stopped." << std::endl;
    }

    return result;
}
