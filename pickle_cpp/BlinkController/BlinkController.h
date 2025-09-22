// BlinkController.h

#ifndef BLINKCONTROLLER_H
#define BLINKCONTROLLER_H

#include <iostream>
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Team/Team.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../Observer/Observer.h"

class TeamStatus {
public:
    bool isPaired = false;

    void toggleLED( PinInterface* pinInterface, int ledPin, bool toggledState );
};

class BlinkController : public Observer {
public:
    BlinkController( PinInterface* pinInterface, Team* team_a, Team* team_b );

    void toggleLEDsBasedOnTime();
    void update( Subject* teamArg );
    void updateLEDs();

private:
    PinInterface* _pinInterface;
    bool _toggledState;
    unsigned long _lastToggleTime;
    unsigned long _time_on_interval;
    unsigned long _time_off_interval;
    TeamStatus _green_team;
    TeamStatus _red_team;
    Team* _team_a;
    Team* _team_b;
};

#endif // BLINKCONTROLLER_H