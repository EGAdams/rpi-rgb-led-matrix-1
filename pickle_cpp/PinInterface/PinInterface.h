#pragma once
#ifndef PININTERFACE_H
#define PININTERFACE_H

#include "../PinState/PinState.h"
#include "../Arduino/Arduino.h"
#if !defined( ARDUINO_TEENSY41 )
    #include <iostream>
    #include <string>
    #include "../TranslateConstant/TranslateConstant.h"
    #include "../Logger/Logger.h"
    #include <fstream>
#endif

#define MCP23017_ADDRESS    0x20  // Default I2C address when A0, A1, A2 are tied to GND
#define I2C_DEVICE          "/dev/i2c-1"
#define GPIO_REGISTER       0x12  // GPIOA register for reading pins


// for the expander
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// to set the bits
#include <bitset>

class PinInterface {
public:
    PinInterface( PinState* pinState );
    ~PinInterface();
    void pinAnalogWrite( int pin, int value );
    void pinDigitalWrite( int pin, int value );
    int pinAnalogRead( int pin );
    int pinDigitalRead( int pin );
#if !defined( ARDUINO_TEENSY41 )
    std::map<std::string, int> getPinStateMap();
#endif   
    int read_mcp23017_value();

private:
    PinState* _pinState;
    #if !defined( ARDUINO_TEENSY41 )
        TranslateConstant* _translateConstant;
        Logger* _logger;
    #endif
    int _readBits_0_4( int file );
};

#endif
