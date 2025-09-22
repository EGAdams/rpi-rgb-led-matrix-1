#ifndef PINSTATE_H
#define PINSTATE_H

#if !defined( ARDUINO_TEENSY41 )
#include <map>
#include <string>
#include "../TranslateConstant/TranslateConstant.h"
#endif

class PinState {
public:
    PinState();
    ~PinState();
#if !defined( ARDUINO_TEENSY41 )
    void setPinState( std::string pin, int state );
    int getPinState( std::string pin );
    void clear();
    std::map< std::string, int > getMap();
#endif

private:
#if !defined( ARDUINO_TEENSY41 )
    std::map< std::string, int > _pin_map;
    TranslateConstant _translateConstant;
#endif    
};

#endif
