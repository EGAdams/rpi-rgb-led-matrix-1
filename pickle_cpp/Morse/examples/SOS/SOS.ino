/*
 * Entry point for Arduino sketches
 */
#include <Command.h>
#include <DIGI_V6_15.h>
#include <GAME_MODES.h>
#include <GameObject.h>
#include <GameState.h>
#include <GameTimer.h>
#include <INPUTS.h>
#include <Logger.h>
#include <Morse.h>
#include <PinInterface.h>
#include <PinState.h>
#include <Player.h>
#include <RESET.h>
#include <Rules.h>
#include <ScoreBoard.h>
#include <ScoreCommand.h>
#include <Team.h>

// #include <UNDO.h>
// #include <History.h>

#if defined _WIN32 || defined _WIN64
    void pinMode( int, int ){}
    #define INPUT 100
    class Serial;
#endif

void setup() {
    pinMode( PLAYER_BUTTONS,  INPUT  );
    pinMode( ROTARY,          INPUT  );
    // pinMode( UNDO,            INPUT  );
    pinMode( RESET,           INPUT  ); 
    pinMode( REMOTE_DATA_0,   INPUT  );
    pinMode( REMOTE_DATA_1,   INPUT  );
    pinMode( REMOTE_DATA_2,   INPUT  );
    pinMode( REMOTE_DATA_3,   INPUT  );
    pinMode( TEAM_A_SERVE_1,  OUTPUT );
    pinMode( TEAM_A_SERVE_2,  OUTPUT );
    pinMode( TEAM_B_SERVE_1,  OUTPUT );
    pinMode( TEAM_B_SERVE_2,  OUTPUT ); 
    pinMode( TEAM_A_POINT_0,  OUTPUT );
    pinMode( TEAM_A_POINT_1,  OUTPUT );
    pinMode( TEAM_A_POINT_2,  OUTPUT );
    pinMode( TEAM_A_POINT_3,  OUTPUT ); 
    pinMode( TEAM_A_POINT_4,  OUTPUT );
    pinMode( TEAM_A_POINT_5,  OUTPUT ); 
    pinMode( TEAM_A_POINT_6,  OUTPUT );
    pinMode( TEAM_A_POINT_7,  OUTPUT );
    pinMode( TEAM_A_POINT_8,  OUTPUT );
    pinMode( TEAM_A_POINT_9,  OUTPUT );
    pinMode( TEAM_A_POINT_10, OUTPUT );
    pinMode( TEAM_B_POINT_0,  OUTPUT );
    pinMode( TEAM_B_POINT_1,  OUTPUT );
    pinMode( TEAM_B_POINT_2,  OUTPUT ); 
    pinMode( TEAM_B_POINT_3,  OUTPUT );
    pinMode( TEAM_B_POINT_4,  OUTPUT );
    pinMode( TEAM_B_POINT_5,  OUTPUT );
    pinMode( TEAM_B_POINT_6,  OUTPUT );
    pinMode( TEAM_B_POINT_7,  OUTPUT );
    pinMode( TEAM_B_POINT_8,  OUTPUT );
    pinMode( TEAM_B_POINT_9,  OUTPUT );
    pinMode( TEAM_B_POINT_10, OUTPUT ); 
    pinMode( A_SET_1        , OUTPUT );
    pinMode( A_SET_2        , OUTPUT ); 
    pinMode( B_SET_1        , OUTPUT );
    pinMode( B_SET_2        , OUTPUT );

    Serial.begin( 9600 ); }

GameObject gameObject;

void loop() { gameObject.loopGame(); }

/* 
 * PCB_2_PB_v87_change_3rd_game_rules_to_go_to_5_not_11
 * Version: 86
 * 
 * Wednesday March 17, 2024 1:45pm
 * 
 * Change the rule for 3rd game to go to 5 by 2 instead of 11 by 2.
 * 
 */
