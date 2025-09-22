# Who you are
You are an Expert C++ Developer

# What we are doing
We are making a blinking controller for LEDs that blink while TEAM_A and TEAM_B pair their remotes.
These are the rules:
## Rules
- At the start, no TEAMs are PAIRED and their respective LEDs should blink.
- When the button for TEAM_A is pressed, TEAM_A pairing should stop and the TEAM_A_POINT_0 LED should go OFF
- When the button for TEAM_B is pressed, TEAM_B paring should stop and the TEAM_B_POINT_0 LED should go OFF.

- If both TEAMs are PAIRED, both TEAM_A_POINT_0 and TEAM_B_POINT_0 LEDs should be OFF.


# C++ Source code to analyze
## BlinkController.h
```cpp
#ifndef BLINKCONTROLLER_H
#define BLINKCONTROLLER_H

#include "GameTimer.h"
#include "PinInterface.h"
#include "Team.h"
#include "DIGI_V6_15.h"
#include "Observer.h"

class TeamStatus {
public:
    bool isPaired = false;

    void toggleLED(PinInterface* pinInterface, int ledPin, bool toggledState);
};

class BlinkController : public Observer {
public:
    BlinkController(PinInterface* pinInterface);

    void toggleLEDsBasedOnTime();
    void update(Subject* teamArg);
    void updateLEDs();

private:
    PinInterface* _pinInterface;
    bool _toggledState;
    int _lastToggleTime;
    int _time_on_interval;
    int _time_off_interval;
    TeamStatus _green_team;
    TeamStatus _red_team;
};

#endif // BLINKCONTROLLER_H


```
## BlinkController.cpp
```cpp
#include "BlinkController.h"

// Definition for TeamStatus::toggleLED function
void TeamStatus::toggleLED(PinInterface* pinInterface, int ledPin, bool toggledState) {
    pinInterface->pinDigitalWrite(ledPin, toggledState ? 1 : 0);
}

// Constructor for the BlinkController class
BlinkController::BlinkController(PinInterface* pinInterface)
    : _pinInterface(pinInterface), _toggledState(false), _lastToggleTime(0),
      _time_on_interval(3000), _time_off_interval(2000) {}

// Method to toggle LEDs based on elapsed time
void BlinkController::toggleLEDsBasedOnTime() {
    int currentTime = GameTimer::gameMillis();
    if (currentTime - _lastToggleTime >= (_toggledState ? _time_off_interval : _time_on_interval)) {
        _toggledState = !_toggledState;
        _lastToggleTime = currentTime;
        updateLEDs();
    }
}

// Method to update the controller based on the team information
void BlinkController::update(Subject* teamArg) {
    Team* team = static_cast<Team*>(teamArg);
    TeamStatus& teamStatus = team->number() == TEAM_A ? _green_team : _red_team;
    teamStatus.isPaired = team->getRemotePaired();

    if (teamStatus.isPaired) {
        _pinInterface->pinDigitalWrite(team->number() == TEAM_A ? TEAM_A_POINT_0 : TEAM_B_POINT_0, 0);
    } else {
        toggleLEDsBasedOnTime();
    }
}

// Method to update LEDs based on the current toggled state
void BlinkController::updateLEDs() {
    if (!_green_team.isPaired) {
        _green_team.toggleLED(_pinInterface, TEAM_A_POINT_0, _toggledState);
    }
    if (!_red_team.isPaired) {
        _red_team.toggleLED(_pinInterface, TEAM_B_POINT_0, _toggledState);
    }
}


```

# Your Task
- TEAM_A_POINT_0 and TEAM_B_POINT_0 LEDs stay on.  Please rewrite the code to make them toggle like they are supposed to.
- Also, liberally sprinkle std::cout statements everywhere so that we can debug this if needed.

# Extra instructions
- NO ABREVIATIONS!   NO "Existing code .... " in the comments.  
- WRITE OUT THE ENTIRE SOLUTIONS.  I HAVE NO WAY TO MODIFY IT.  
- CODE ONLY PLEASE.
