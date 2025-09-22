// BlinkController.cpp

#include "BlinkController.h"

#define TIME_ON_INTERVAL 2000  // looks like 2 seconds
#define TIME_OFF_INTERVAL 500  // looks like 1 second for some reason

void TeamStatus::toggleLED( PinInterface* pinInterface, int ledPin, bool toggledState ) {
    pinInterface->pinDigitalWrite( ledPin, toggledState ? 1 : 0 );
}

BlinkController::BlinkController( PinInterface* pinInterface, Team* team_a, Team* team_b )
    : _pinInterface( pinInterface ), _toggledState( false ), _lastToggleTime( 0 ),
      _time_on_interval( TIME_ON_INTERVAL ), _time_off_interval( TIME_OFF_INTERVAL ),
      _team_a( team_a ), _team_b( team_b ) {
}

void BlinkController::toggleLEDsBasedOnTime() {
    unsigned long currentTime = GameTimer::gameMillis();
    unsigned long delta_time = currentTime - _lastToggleTime;
    if ( delta_time >= ( _toggledState ? _time_on_interval : _time_off_interval )) {
        _toggledState = !_toggledState;
        _lastToggleTime = currentTime;
        updateLEDs();
    }
}

void BlinkController::update( Subject* teamArg ) {
    Team* team = static_cast<Team*>( teamArg );
    TeamStatus& teamStatus = team->number() == TEAM_A ? _green_team : _red_team;
    teamStatus.isPaired = team->getRemotePaired();
    if ( teamStatus.isPaired ) {
        _pinInterface->pinDigitalWrite( team->number() == TEAM_A ? TEAM_A_POINT_0 : TEAM_B_POINT_0, 0 );
    } else {
        toggleLEDsBasedOnTime();
    }
}

void BlinkController::updateLEDs() {
    if ( !_green_team.isPaired ) {
        if( _team_a->getRemotePaired()) {
            _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 0 );
        } else {
            _green_team.toggleLED( _pinInterface, TEAM_A_POINT_0, _toggledState );
        }
    } else { 
        _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 0 );
    }
    if ( !_red_team.isPaired ) {
        if( _team_b->getRemotePaired()) {
            _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 0 );
        } else {
            _red_team.toggleLED( _pinInterface, TEAM_B_POINT_0, _toggledState );
        }
    } else {
        _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 0 );
    }
}
