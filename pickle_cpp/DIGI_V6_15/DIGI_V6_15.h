#include <thread>

#ifndef DIGI_V6_15_H
#define DIGI_V6_15_H

#include <iomanip>

#if defined( ARDUINO_TEENSY41 )
#include "Arduino.h"
#else
#include <cstring>
#endif

#define print( x ) \
    do { \
        auto now = std::chrono::system_clock::now(); \
        auto in_time_t = std::chrono::system_clock::to_time_t( now ); \
        std::tm buf; \
        localtime_r( &in_time_t, &buf ); \
        const char* file = strrchr(__FILE__, '/'); \
        file = file ? file + 1 : __FILE__; \
        std::cout << "[" << std::put_time( &buf, "%H:%M:%S" ) << "] " \
                  << "[" << file << "] " \
                  << "[: " << __LINE__ << "] " \
                  << "[" << __FUNCTION__ << "()] " \
                  << "[ Thread ID: " << std::this_thread::get_id() << " ] " \
                  << x << std::endl; \
    } while ( 0 )
#endif
//
//  Written by EG Adams for Teensy 4.1 only
//
// #define INPUT 0 // not in windows.. ouch!!
// #define OUTPUT 0
#define HOME_DIRECTORY "/home/dietpi"
#define LOW  0
#define HIGH 1

#define MAX_POINTS 21

// pairing screen vertical offsets for centering
#define GREEN_OFFSET 4
#define RED_OFFSET 12
#define YOUR_OFFSET 7
#define PRESS_OFFSET 4
#define REMOTE_OFFSET 0
#define BUTTON_OFFSET 1

#define PLAYER_TOP 30
#define PRESS_TOP 47
#define YOUR_TOP 60
#define REMOTE_TOP 73
#define MIDDLE_OFFSET 2
#define GREEN_TOP 94
#define RED_TOP 94
#define BUTTON_TOP 107

#define PB_BIG_NUMBER_FONT "fonts/pickle_zero_sliced.bdf"
#define PB_LITTLE_NUMBER_FONT "fonts/10x20.bdf"

#define SETS_TO_WIN_MATCH 2

#define SET_WINNER_FLASH_DELAY       1000
#define MATCH_WINNER_SET_FLASH_DELAY 500 // 200 changed to 500 on easter tuesday

#define MAIN_INPUT_TIMEOUT              120000 // 20000
#define REGULAR_GAME_PLAY_INPUT_TIMEOUT MAIN_INPUT_TIMEOUT
#define INPUT_TIMEOUT_CODE              99
#define MATCH_WIN_FLASH_DELAY           1000
#define MATCH_WIN_BLINK_COUNT           5
#define PLAYER_ONE_SET_INDEX            1 // this is the number for _team_a
#define PLAYER_TWO_SET_INDEX            2 // this is the number for _team_b 
#define SLEEP_FOREVER                   100000000000  // a little over 3 years
#define AFTER_MATCH_UNDO_TIMEOUT_MS     15000  // 15 second timeout for after match win show and undo

///////////////////////////////////////////////////////////////////////////////

#if !defined( ARDUINO_TEENSY41 )
    #define SIMULATION 1
    #define PLAYER_BUTTONS 202  // A2 // changed from A2 to rid of compiler warning
    #define ROTARY 150  // A15  // "15" has TAP2 conflict! // changed from A15 to rid of compiler error
    #define PIN_FILE_PATH ""
    #define LOG_FILE_PATH ""
    #define TEAM_A_ZERO_POINT 22 // 34 and 35 don't exist on simulator yet.
    #define TEAM_B_ZERO_POINT 20 // 35
    #define MATCH_WINNER_FLASH_DELAY 500 // otherwise it is too fast in the simulator. 
#else
    #define SIMULATION 0
    #define A2 2
    #define A15 15
    #define PLAYER_BUTTONS A2 // A2 // changed from A2 to rid of compiler warning
    #define ROTARY A15 // A15         // changed from A15 to rid of compiler error
    #define TEAM_A_ZERO_POINT 19 // 22 // 34 and 35 don't exist on simulator yet.
    #define TEAM_B_ZERO_POINT 0  // 20
    #define MATCH_WINNER_FLASH_DELAY 200  // moved on easter wednesday, 2024
#endif


#define ZERO_CONDITION_BLINK_DELAY  1500
#define MAX_SLEEP                   20
#define SCORE_DELAY_IN_MILLISECONDS 50

////////////////// states //////////////////

#define REGULAR_PLAY_NO_SCORE_STATE     1
#define NO_SCORE_SLEEP_STATE            2
#define AFTER_SLEEP_MODE                3
#define REGULAR_PLAY_SLEEP_STATE        4
#define REGULAR_PLAY_AFTER_SCORE_STATE  5
#define PAIRING_SLEEP_MODE_STATE        6
#define PAIRING_MODE_STATE              7
#define SLEEP_MODE                      8
#define MATCH_WIN_BLINK_STATE           9
#define AFTER_MATCH_WIN_STATE           10
#define UNDO_STATE                      11
////////////////////////////////////////////

#define REMOTE_DATA_0 25
#define REMOTE_DATA_1 24
#define REMOTE_DATA_2 40
#define REMOTE_DATA_3 17

#define REMOTE_INPUT  1

// #define RED_REMOTE_RED_SCORE     1 
#define RED_REMOTE_RED_SCORE     6  // 072325
#define RED_REMOTE_GREEN_SCORE   10
#define RED_REMOTE_UNDO          2
#define RED_REMOTE_RESET         4
// #define GREEN_REMOTE_GREEN_SCORE 5
#define GREEN_REMOTE_GREEN_SCORE 7 // 072325
#define GREEN_REMOTE_RED_SCORE   11 // 6  072325
#define GREEN_REMOTE_UNDO        3 // 7  072325
#define GREEN_REMOTE_RESET       8 // go copilot for those last 3!
#define NO_REMOTE_BUTTONS        0
#define UNKNOWN_REMOTE_BUTTON   99
#define UMPIRE_REMOTE_GREEN_SCORE 5
#define UMPIRE_REMOTE_RED_SCORE   9
#define UMPIRE_REMOTE_UNDO        1
#define NO_REMOTE_BUTTONS         0

#define INPUT_TIMEOUT_CODE        99

#define STEVE_DELAY            25
#define REMOTE_READ_DELAY      25
#define MAIN_GAME_DELAY        10

#define SINGLES_MODE 1
#define DOUBLES_MODE 2
#define START_SERVE_NUMBER 1

#define UNDO   40000
#define RESET  38
#define GREEN_TEAM 1
#define RED_TEAM 2
#define TEAM_A 1
#define TEAM_B 2
#define SET_WINNER_FLASH_COUNT       5
#define MATCH_WINNER_FLASH_COUNT     5
#define MATCH_WINNER_SET_FLASH_COUNT 8 // 3 changed on easter tuesday
#define GAME_LAMP_COUNT              12 // 11 this should to it...
#define SET_WINNER_ON_TIME           1500
#define SET_WINNER_OFF_TIME          1000
#define SET_WINNER_TOTAL_BLINK_TIME  8000
#define GAME_3_POINTS_TO_WIN         3

#define TEAM_A_SERVE_1  23
#define TEAM_A_SERVE_2  22 
#define TEAM_B_SERVE_1  21
#define TEAM_B_SERVE_2  20
#define TEAM_A_POINT_0  19
#define TEAM_A_POINT_1  18 // was 37 june 12 fix
#define TEAM_A_POINT_2  15 // was 41 june 12 fix
#define TEAM_A_POINT_3  14
#define TEAM_A_POINT_4  41 // was 15 june 12 fix
#define TEAM_A_POINT_5  37 // was 18 june 12 fix
#define TEAM_A_POINT_6  36 // was 3 june 12 fix // not known yet (for sure).
#define TEAM_A_POINT_7  29 // 33 changed on March 11, 2024 // was 25 june 12 fix 
#define TEAM_A_POINT_8  30 // 29 changed on March 11, 2024
#define TEAM_A_POINT_9  33 // 30 changed on March 11, 2024 // was 33 june 12 fix
#define TEAM_A_POINT_10 31 // was 36 june 12 fix
#define TEAM_B_POINT_0  0  // was 4 june 12 fix
#define TEAM_B_POINT_1  1  // was 3 june 12 fix
#define TEAM_B_POINT_2  2
#define TEAM_B_POINT_3  3  // was 1 june 12 fix
#define TEAM_B_POINT_4  4  // was 0 june 12 fix
#define TEAM_B_POINT_5  5  // was 9 june 12 fix
#define TEAM_B_POINT_6  6  // was 8 june 12 fix
#define TEAM_B_POINT_7  7  
#define TEAM_B_POINT_8  8  // was 6 june 12 fix
#define TEAM_B_POINT_9  9  // was 5 june 12 fix
#define TEAM_B_POINT_10 10 // // was 31 june 12 fix.  not known yet (for sure).
#define A_SET_1 28 // was 11 june 12 fix // was 12.  swapped with b_set_2 on june 15     
#define A_SET_2 32 // was 36 june 12 fix  
#define B_SET_1 11 // was 12 june 12 fix     
#define B_SET_2 12 // was 28 swapped with a_set_1 on june 15
#define SET_2_ONLY 3 // used for the match win display loop
