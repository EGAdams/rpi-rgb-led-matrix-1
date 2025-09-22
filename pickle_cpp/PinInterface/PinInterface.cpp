#include "PinInterface.h"
#include <map>
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../GameTimer/GameTimer.h"

PinInterface::PinInterface( PinState* pinState ) : _pinState( pinState ) {
    #if !defined(ARDUINO_TEENSY41)
    _translateConstant = new TranslateConstant();
    #endif
}
PinInterface::~PinInterface() {}

// Function to read bits 0 to 4 from the expander
int PinInterface::_readBits_0_4( int file ) {
    // print( "inside _readBits_0_4" );
    uint8_t reg = GPIO_REGISTER;
    uint8_t value;

    if ( write( file, &reg, 1 ) != 1 ) {
        std::cerr << "Error: Unable to write to I2C device.\n";
        return -1;
    }

    if ( read( file, &value, 1 ) != 1 ) {
        std::cerr << "Error: Unable to read from I2C device.\n";
        return -1;
    }

    uint8_t bits = value & 0x1F;  // Mask bits 0-4

    // print( "Decimal value: " << static_cast< int >( bits ));
    if ( static_cast< int >( bits ) != 15 ) {
        // print( "Binary value: " << std::bitset<5>( bits ));
    }
    // print( "returning bits " << static_cast< int >( bits ) << " to the PinInterface read_mcp23017_value method..." );
    return static_cast< int >( bits );
}

int PinInterface::read_mcp23017_value() {
    // print( "reading MCP23017 bits..." );
    int file = open( I2C_DEVICE, O_RDWR );

    if ( file < 0 ) {
        std::cerr << "Error: Unable to open I2C device.\n";
        return -1;
    }

    if ( ioctl( file, I2C_SLAVE, MCP23017_ADDRESS ) < 0 ) {
        std::cerr << "Error: Unable to set I2C address.\n";
        close( file );
        return -1;
    }
    // print( "calling _readBits_0_4 to read MCP23017 bits 0 to 4..." );
    int bits = _readBits_0_4( file );
    close( file );
    // print( "returning bits from read_mcp23017_value(): " << std::to_string( bits ));
    return bits;
}

int PinInterface::pinAnalogRead( int pin ) {
    #if !defined(ARDUINO_TEENSY41)
        std::ifstream dataIn;
        std::string value_read_from_file;
        GameTimer::gameDelay( 20 );
        dataIn.open( PIN_FILE_PATH + _translateConstant->get_translated_constant( pin ) + ".txt" );
        GameTimer::gameDelay( 20 );
        dataIn >> value_read_from_file;
        if( value_read_from_file.length() == 0 ) {
            std::cout << "*** WARNING nothing read from PIN_FILE_PATH ***  setting to 1..." << std::endl;
            value_read_from_file = "1";
        }
        dataIn.close();
        GameTimer::gameDelay( 20 );
        int pin_value = std::stoi( value_read_from_file );
    #else
        int pin_value = analogRead( pin );
    #endif
    return pin_value;
}

int PinInterface::pinDigitalRead( int pin ) {
    #if !defined( ARDUINO_TEENSY41 )
    int pin_value = _pinState->getPinState( std::to_string( pin ));
    #else
    int pin_value = digitalRead( pin );
    #endif
    return pin_value;
}

void PinInterface::pinAnalogWrite( int pin, int value ) {
    #if !defined( ARDUINO_TEENSY41 )
    _pinState->setPinState( std::to_string( pin ), value );
    #else
    analogWrite( pin, value );
    #endif
}

void PinInterface::pinDigitalWrite( int pin, int value ) {
    #if !defined( ARDUINO_TEENSY41 )
    _pinState->setPinState( std::to_string( pin ), value );
    #else 
    digitalWrite( pin, value );
    #endif
}

#if !defined( ARDUINO_TEENSY41 )
std::map<std::string, int> PinInterface::getPinStateMap() {
    return _pinState->getMap();
}
#endif
