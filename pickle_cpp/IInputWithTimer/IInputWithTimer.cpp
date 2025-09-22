#include "IInputWithTimer.h"

unsigned long IInputWithTimer::getTimeSlept() const { return _time_slept; }

void IInputWithTimer::setTimeout( unsigned long timeout ) { _timeout_ms = timeout; }
