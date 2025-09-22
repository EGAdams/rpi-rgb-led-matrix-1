#ifndef INPUTS_h
#define INPUTS_h

#include "../Arduino/Arduino.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../RESET/RESET.h"
#include "../Team/Team.h"
#include "../GameStateManager/GameStateManager.h"
#include "../RemoteCodeTranslator/RemoteCodeTranslator.h"
#if !defined( ARDUINO_TEENSY41 )
#include "../Logger/Logger.h"
#include <iostream>
#endif

class History;
class ScoreBoard;

class Inputs {
public:
    Inputs( Team* team_a, Team* team_b, PinInterface* pinInterface, ScoreBoard* scoreBoard );
    ~Inputs();
    void readReset( Reset* reset, GameState* gameState, History& history );
    void readUndoButton( GameState* gameState );
    int readRotary( GameState* gameState, Reset* reset );
    int readPlayerButtons();
    int readRemotePinArray( remoteDataStructure* remotePinData );
    int read_mcp23017_value();

private:
    Team* _team_a;
    Team* _team_b;
    PinInterface* _pinInterface;
    GameStateManager* _gameStateManager;
    RemoteCodeTranslator* _remoteCodeTranslator;
    ScoreBoard* _scoreboard;
    //
    #if !defined( ARDUINO_TEENSY41 )
    Logger* _logger;
    #endif
};

#endif
