#ifndef Logger_h
#define Logger_h

#if !defined( ARDUINO_TEENSY41 )
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include "../GameTimer/GameTimer.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"

class Logger {
public:
    Logger( std::string constructor_name );
    ~Logger();
    void logUpdate( std::string message, std::string caller );
    decltype( std::chrono::seconds().count() ) _get_seconds_since_epoch();
    int _get_random_number();
    bool inArray( int supposed_random_number );

private:
    std::string _constructor_name;
    std::ofstream _log_file;
    std::vector<int> _used_random_numbers;
};
#else
class Logger {
public:
    Logger();
    ~Logger();
};
#endif  // defined _WIN*

#endif  // Logger_h