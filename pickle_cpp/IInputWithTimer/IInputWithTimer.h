#ifndef I_INPUT_WITH_TIMER_H
#define I_INPUT_WITH_TIMER_H
#include "../Blinker/Blinker.h"
#include <iostream>

class IInputWithTimer {
protected:
    Blinker* _blinker;
    unsigned long _timeout_ms;  // Timeout duration in milliseconds
    unsigned long _time_slept;  // Time slept in milliseconds

    IInputWithTimer(Blinker* blinker, unsigned long timeout_ms)
        : _blinker(blinker), _timeout_ms(timeout_ms), _time_slept(0) {
            std::cout << "IInputWithTimer constructor called" << std::endl;
            // check if blinker is null
            if (blinker == nullptr) {
                std::cout << "*** ERROR: blinker is null! ***" << std::endl;
            } else {
                std::cout << "*** blinker is not null, continuing... ***" << std::endl;
            }
        }

public:
    virtual ~IInputWithTimer() = default;
    // Pure virtual methods to enforce implementation in derived classes
    virtual int getInput() = 0;
    unsigned long getTimeSlept() const;
    void setTimeout(unsigned long timeout);
};

#endif // I_INPUT_WITH_TIMER_H