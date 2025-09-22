/*
 * class TestGameInput
 */
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../TestGameInput/TestGameInput.h"
#include <fstream>
#include <sstream>
#include <algorithm>

TestGameInput::~TestGameInput() { std::cout << "destroying test game input..."  << std::endl; }
TestGameInput::TestGameInput() {  
    std::cout << "constructing test game input..." << std::endl;

    // Initialize the input map with the defines from instructions
    _input_map = {
        { "RED_REMOTE_RED_SCORE",       1  },
        { "RED_REMOTE_GREEN_SCORE",     2  },
        { "RED_REMOTE_UNDO",            3  },
        { "RED_REMOTE_RESET",           4  },
        { "GREEN_REMOTE_GREEN_SCORE",   5  },
        { "GREEN_REMOTE_RED_SCORE",     6  },
        { "GREEN_REMOTE_UNDO",          7  },
        { "GREEN_REMOTE_RESET",         8  },
        { "UMPIRE_REMOTE_GREEN_SCORE",  5  },
        { "UMPIRE_REMOTE_RED_SCORE",    9  },
        { "UMPIRE_REMOTE_UNDO",         1  },
        { "NO_REMOTE_BUTTONS",          0  },
        { "UNKNOWN_REMOTE_BUTTON",      15 }
    };

    // Load test inputs from file
    if ( _onRaspberryPi()) {
        std::cout << "loading test inputs from /home/dietpi/rpi-rgb-led-matrix/pickle_cpp/TestGameInput/test_input.txt" << std::endl;
        _loadTestInputs( "/home/dietpi/rpi-rgb-led-matrix/pickle_cpp/TestGameInput/test_input.txt" );
    } else {
        std::cout << "loading test inputs from /home/adamsl/rpi-rgb-led-matrix/pickle_cpp/TestGameInput/test_input.txt" << std::endl;
        _loadTestInputs( "/home/adamsl/rpi-rgb-led-matrix/pickle_cpp/TestGameInput/test_input.txt" );
    } 
}

void TestGameInput::_loadTestInputs( const std::string& filename ) {
    std::ifstream infile( filename );
    if ( !infile.is_open()) {
        std::cerr << "Failed to open test input file: " << filename << std::endl;
        return;
    }

    std::string line;
    while ( std::getline( infile, line )) {
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

        // Look up the line in _input_map
        auto it = _input_map.find( line );
        if ( it != _input_map.end()) {
            _test_inputs.push_back( it->second );
        }
        else {
            std::cerr << "Unknown input in test file: " << line << std::endl;
            _test_inputs.push_back( _input_map[ "UNKNOWN_REMOTE_BUTTON" ]);
        }
    }
}

int TestGameInput::getInput() {
    if ( _current_index < _test_inputs.size()) {
        std::cout << "next on stack: " << _test_inputs[ _current_index ] << std::endl;
        return _test_inputs[ _current_index++ ];
    }
    else {
        // End of test inputs, return NO_REMOTE_BUTTONS
        // return _input_map[ "NO_REMOTE_BUTTONS" ];
        std::cout << "End of Tests.  <enter> to exit." << std::endl;
        exit( 0 );
    }
}

bool TestGameInput::_onRaspberryPi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}