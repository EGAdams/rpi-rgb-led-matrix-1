/** @class PinState
 *  This class is only used in the Windows simulator.
 *  In Arduino, it is only used as a placeholder to construct the PinInterface class.
 */
#include "PinState.h"
#if !defined( ARDUINO_TEENSY41 )
#include <fstream>
#include <iostream>
#endif
PinState::PinState() {}
PinState::~PinState() {}
#if !defined( ARDUINO_TEENSY41 )
int PinState::getPinState( std::string pin ) {
    return _pin_map[ pin ]; }

void PinState::setPinState( std::string pin, int state ) {
    _pin_map[ pin ] = state;
    std::string pin_file_path = PIN_FILE_PATH;
    std::ofstream pin_file( pin_file_path + _translateConstant.get_translated_constant( std::stoi( pin )) + ".txt" );
    pin_file << state;
    pin_file.close(); }

void PinState::clear() { _pin_map.clear(); }

std::map<std::string, int> PinState::getMap() { return _pin_map; }
#endif
