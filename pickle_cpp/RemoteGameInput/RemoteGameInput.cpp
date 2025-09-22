#include "RemoteGameInput.h"

RemoteGameInput::RemoteGameInput( IInputs* inputs ) : _inputs( inputs ) {
    std::cout << "constructing remote game input..." << std::endl;
}

RemoteGameInput::~RemoteGameInput() {
    std::cout << "destroying remote game input..." << std::endl;
}

int RemoteGameInput::getInput() {
    // std::cout << "\n\n*** Getting input from inside remote game input... ***\n\n" );
    int input_read_from_ic = _inputs->read_mcp23017_value();
    if ( input_read_from_ic != 15 ) {
        // std::cout << "*** returing input_read_from_ic = " << std::to_string( input_read_from_ic ) << " ***\n\n" );
    }
    return input_read_from_ic;
}