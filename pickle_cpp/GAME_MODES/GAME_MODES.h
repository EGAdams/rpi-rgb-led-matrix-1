#ifndef GAME_MODES_h
#define GAME_MODES_h
#include "../Arduino/Arduino.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"

#include "../GameTimer/GameTimer.h"
#include "../INPUTS/INPUTS.h"
#include "../PinInterface/PinInterface.h"
#include "../Team/Team.h"
#include "../Rules/Rules.h"
#include "../ScoreCommand/ScoreCommand.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../RESET/RESET.h"
#include "../RemoteLocker/RemoteLocker.h"
#if !defined(ARDUINO_TEENSY41)
#include <iostream>
#endif
class History;
class GameModes {
public:
    GameModes( Team* team_a,
        Team* team_b,
        PinInterface* pinInterface,
        Rules* rules,
        ScoreBoard* scoreBoard );
    ~GameModes();
    void gameStart( GameState* gameState, History& history );
    void mode1(     GameState* gameState, History& history );
    void mode2(     GameState* gameState, History& history );
    void mode4(     GameState* gameState, History& history );
    void undo(      GameState* gameState, History& history );
    void noCode();
    void setGameMode( int rotaryPosition,  GameState* gameState, History& history );

private:
    Team* _team_a;
    Team* _team_b;
    PinInterface* _pinInterface;
    Rules* _rules;
    ScoreBoard* _scoreBoard;
    Inputs     _inputs;
    GameStateManager* _gameStateManager;
    Reset _reset;
    RemoteLocker* _remoteLocker;
};

#endif
