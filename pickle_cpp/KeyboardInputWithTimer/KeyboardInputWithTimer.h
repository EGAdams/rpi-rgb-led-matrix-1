#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <chrono>
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../IInputWithTimer/IInputWithTimer.h"

/**
 * @brief A class for handling non-blocking keyboard input with timeout functionality.
 * 
 * KeyboardInputWithTimer allows for polling keyboard input while enforcing a timeout period. 
 * It includes methods to get elapsed time during the polling process.
 */
class KeyboardInputWithTimer : public IInputWithTimer {
public:
    KeyboardInputWithTimer( Blinker* blinker, unsigned long timeout = 4000 );
    ~KeyboardInputWithTimer();

    /**
     * @brief Gets keyboard input with timeout functionality.
     * 
     * Continuously polls for keyboard input until valid input is received or timeout occurs.
     * Valid input values are 1, 2, or 9. Returns a timeout value if no input is received.
     * 
     * @return int The valid input value or a timeout value (-1).
     */
    int getInput() override;

    /**
     * @brief Sets the timeout duration for keyboard input.
     * @param timeout The timeout duration in miliseconds (default is 4000).
     */
   

private:
    /**
     * @brief Validates the user input against allowed values.
     * @param selection The input value to validate.
     * @return bool True if the input is valid, false otherwise.
     */
    bool validateInput(int selection) const;
    std::chrono::steady_clock::time_point _startTime; ///< Start time for input polling.
    unsigned long _elapsedTimeMs; ///< Elapsed time in timeout.
    void _restoreTerminal( const struct termios& oldt, int old_flags );
    int _configureTerminal( struct termios& oldt );
};

#endif // KEYBOARD_INPUT_H
