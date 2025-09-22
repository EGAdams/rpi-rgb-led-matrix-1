#include "PairingBlinker.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <stdexcept>

PairingBlinker::PairingBlinker( ScoreBoard* scoreBoard )
    : _scoreboard( scoreBoard ), _should_stop( false ), _green_player_paired( false ), _red_player_paired( false ) {
    std::cout << "PairingBlinker constructing..." << std::endl;
    if ( _scoreboard == nullptr ) { // check for null _scoreboard
        std::cerr << "Error: _scoreboard is null in PairingBlinker constructor." << std::endl;
        return;  // TODO: louder error and exit!
    }
}

PairingBlinker::~PairingBlinker() { stop(); }

void PairingBlinker::blinkLoop() {
    bool toggle_on = true;  // Start with Green instructions
    print( "starting blink loop..." );
    unsigned long pairing_start_time = GameTimer::gameMillis();
    while ( !_should_stop ) {
        // print( "in blink loop..." );
        unsigned long current_time       = GameTimer::gameMillis();
        // print( "current time: " + std::to_string( current_time ));
        unsigned long elapsed_time       = current_time - pairing_start_time;
        // print( "elapsed time: " + std::to_string( elapsed_time ));
        // Check if timeout exceeded
        if ( elapsed_time > MAIN_INPUT_TIMEOUT ) {
            print( "Timeout exceeded. Switch to blinking ball mode in the future..." );
            print( "set should_stop to true if we are using this as the timer..." );
            _should_stop = true;
        }

        if ( !_scoreboard ) {
            std::cerr << "*** ERROR: _scoreboard is null inside PairingBlinker::blinkLoop()! ***" << std::endl;
        } // return; // or break, just so we exit the blink loop safely

        _scoreboard->clearScreen();
        if ( _green_player_paired && _red_player_paired ) { 
            print( "both players seem to be paired, break..." );
            break;  // If both players are paired, stop blinking
        } else if ( !_green_player_paired && _red_player_paired ) { // If only the Red player is paired, show Green player instructions
            if ( toggle_on ) {
                showGreenInstructions();
                // print( "showing green instructions..." );
            } else {
                // print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            // print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }
        if ( _green_player_paired && !_red_player_paired ) { // If only the Green player is paired, show Red player instructions
            _show_green = false;
            // print( "showing red instructions inside blink loop..." );
            if ( toggle_on ) {
                showRedInstructions();
                // print( "showing red instructions..." );
            }
            else {
                // print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            // print( "alternating between red on and red off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        } else if ( !_green_player_paired && !_red_player_paired ) { // If neither player is paired, show Green instructions
            if ( toggle_on ) {
                showGreenInstructions();
                // print( "showing green instructions..." );
            }
            else {
                // print( "show neutral text only..." );
                showPlayerPressYourRemoteText();
            }
            // print( "alternating between green on and green off..." );
            toggle_on = !toggle_on;  // Alternate led on and led off
        }
        std::this_thread::sleep_for( std::chrono::seconds( 1 )); // Delay for 1 second between switching
    }
    print( "should stop flag must be true, exiting pairing blinker blink loop..." );
}

void PairingBlinker::showGreenInstructions() {
    // std::cout << "*** DEBUG: Starting showGreenInstructions ***" << std::endl;
    
    if (!_scoreboard) {
        std::cerr << "*** ERROR: _scoreboard is null in showGreenInstructions! ***" << std::endl;
        throw std::runtime_error("ScoreBoard is null in showGreenInstructions");
    }
    
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    
    try {
        // std::cout << "*** DEBUG: Clearing screen ***" << std::endl;
        _scoreboard->clearScreen();
        
        // std::cout << "*** DEBUG: Setting drawer foreground color to green ***" << std::endl;
        _scoreboard->setPairingDrawerForegroundColor( green_color );
        
        // std::cout << "*** DEBUG: Drawing Green text at position (" << (left_margin + GREEN_OFFSET) << ", 17) ***" << std::endl;
        _scoreboard->drawPairingText( "Green", left_margin + GREEN_OFFSET, 17 );
        
        // std::cout << "*** DEBUG: Drawing Player text at position (" << (left_margin + 0) << ", " << PLAYER_TOP << ") ***" << std::endl;
        _scoreboard->drawPairingText( "Player", left_margin + 0, PLAYER_TOP );
        
        // std::cout << "*** DEBUG: Setting drawer foreground color to yellow ***" << std::endl;
        _scoreboard->setPairingDrawerForegroundColor( yellow_color );
        
        // std::cout << "*** DEBUG: Drawing Press text ***" << std::endl;
        _scoreboard->drawPairingText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
        
        // std::cout << "*** DEBUG: Drawing Your text ***" << std::endl;
        _scoreboard->drawPairingText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET);
        
        // std::cout << "*** DEBUG: Drawing Remote text ***" << std::endl;
        _scoreboard->drawPairingText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
        
        // std::cout << "*** DEBUG: Setting drawer foreground color back to green ***" << std::endl;
        _scoreboard->setPairingDrawerForegroundColor( green_color );
        
        // std::cout << "*** DEBUG: Drawing final Green text ***" << std::endl;
        _scoreboard->drawPairingText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
        
        // std::cout << "*** DEBUG: Drawing Button text ***" << std::endl;
        _scoreboard->drawPairingText( "Button", left_margin + 0, BUTTON_TOP );
        
        // std::cout << "*** DEBUG: showGreenInstructions completed successfully ***" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "*** ERROR in showGreenInstructions: " << e.what() << " ***" << std::endl;
        throw;
    }
}

void PairingBlinker::showRedInstructions() {
    std::cout << "showing red instructions..." << std::endl;
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    if ( _scoreboard->onRaspberryPi() ) {
        int left_margin = 9;
        _scoreboard->clearScreen();
        _scoreboard->setPairingDrawerForegroundColor( red_color );
        _scoreboard->drawPairingText( "Red", left_margin + RED_OFFSET, 17 );
        _scoreboard->drawPairingText( "Player", left_margin + 0, PLAYER_TOP );
        _scoreboard->setPairingDrawerForegroundColor( yellow_color );
        _scoreboard->drawPairingText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
        _scoreboard->drawPairingText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
        _scoreboard->drawPairingText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
        _scoreboard->setPairingDrawerForegroundColor( red_color );
        _scoreboard->drawPairingText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawPairingText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::showPlayerPressYourRemoteText() {
    std::cout << "showing player press your remote text..." << std::endl;
    Color red_color( 255, 0, 0 );
    Color green_color( 0, 255, 0 );
    Color yellow_color( 255, 255, 0 );
    int left_margin = 9;
    _scoreboard->clearScreen();
    if ( _show_green ) {
        _scoreboard->setPairingDrawerForegroundColor( green_color );
        _scoreboard->drawPairingText( "Player", left_margin + 0, PLAYER_TOP );
    } else {
        _scoreboard->setPairingDrawerForegroundColor( red_color );
        _scoreboard->drawPairingText( "Player", left_margin + 0, PLAYER_TOP );
    }
    _scoreboard->setPairingDrawerForegroundColor( yellow_color );
    _scoreboard->drawPairingText( "Press", left_margin + PRESS_OFFSET, PRESS_TOP + MIDDLE_OFFSET );
    _scoreboard->drawPairingText( "Your", left_margin + YOUR_OFFSET, YOUR_TOP + MIDDLE_OFFSET );
    _scoreboard->drawPairingText( "Remote", left_margin + REMOTE_OFFSET, REMOTE_TOP + MIDDLE_OFFSET );
    if ( _show_green ) {
        _scoreboard->setPairingDrawerForegroundColor( green_color );
        _scoreboard->drawPairingText( "Green", left_margin + GREEN_OFFSET, GREEN_TOP );
        _scoreboard->drawPairingText( "Button", left_margin + 0, BUTTON_TOP );
    } else {
        _scoreboard->setPairingDrawerForegroundColor( red_color );
        _scoreboard->drawPairingText( "Red", left_margin + RED_OFFSET, RED_TOP );
        _scoreboard->drawPairingText( "Button", left_margin + 0, BUTTON_TOP );
    }
}

void PairingBlinker::start() {
    print( "starting blink thread..." );
    _should_stop = false;
    blink_thread = std::thread( &PairingBlinker::blinkLoop, this );
    print( "blink thread started..." );
}

void PairingBlinker::stop() {
    _should_stop = true;
    if ( blink_thread.joinable() ) {
        blink_thread.join();
    }
}

bool PairingBlinker::areBothPlayersPaired() { 
    bool both_paired = ( _green_player_paired && _red_player_paired );
    return both_paired;
}

void PairingBlinker::enable() {            _should_stop = false; }
void PairingBlinker::sleepModeOn() {       _sleep_mode  = true;  }
void PairingBlinker::sleepModeOff() {      _sleep_mode  = false; }
bool PairingBlinker::awake() {     return !_sleep_mode;          }
void PairingBlinker::setGreenPlayerPaired( bool paired ) { _green_player_paired = paired; }
void PairingBlinker::setRedPlayerPaired(   bool paired ) { _red_player_paired = paired;   }
