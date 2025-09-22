#include "../RESET/RESET.h"
#include "../Arduino/Arduino.h"

Reset::Reset( PinInterface* pinInterface ) : _pinInterface( pinInterface ){}
Reset::~Reset() {}

void Reset::resetScoreboard( GameState* gameState ) {
    _pinInterface->pinDigitalWrite( TEAM_A_SERVE_1, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_SERVE_2, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_SERVE_1, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_SERVE_2, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_1, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_2, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_3, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_4, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_5, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_6, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_7, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_8, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_9, LOW );
    _pinInterface->pinDigitalWrite( TEAM_A_POINT_10, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_1, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_2, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_3, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_4, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_5, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_6, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_7, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_8, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_9, LOW );
    _pinInterface->pinDigitalWrite( TEAM_B_POINT_10, LOW );
    _pinInterface->pinDigitalWrite( A_SET_1, LOW );
    _pinInterface->pinDigitalWrite( A_SET_2, LOW );
    _pinInterface->pinDigitalWrite( B_SET_1, LOW );
    _pinInterface->pinDigitalWrite( B_SET_2, LOW );

    gameState->setServe(         0   ); // serve = 0;
    gameState->setPlayerButton(  0   ); // playerButton = 0;
    gameState->setStarted(       0   ); // enables gameStart which resets scores
    GameTimer::gameDelay(        200 ); // delay( 200 );
}

void Reset::refresh( GameState* gameState ) {
    if ( gameState->getRotaryChange() == 1 /* rotaryChange == true */ ) {
        gameState->setRotaryChange( 0 );  // rotaryChange = false;
        resetScoreboard( gameState );
        resetPreviousValues();
    }
}

void Reset::resetPreviousValues() {}
