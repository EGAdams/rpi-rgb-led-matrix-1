/************************************************************
 * pickle_keyboard.cpp
 *
 * Startup file for the PickleBall game using KeyboardGameInput.
 * This file initializes the PickleBall game objects, keyboard input handlers,
 * and runs the main game loop with keyboard controls.
 *
 * Based on run_pickle_listener.cpp but specifically configured for keyboard input.
 ************************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <memory>
#include <fstream>
#include <string>

extern volatile std::sig_atomic_t gSignalStatus;

#include "DIGI_V6_15/DIGI_V6_15.h"
#include "GameObject/GameObject.h"
#include "GameState/GameState.h"
#include "RESET/RESET.h"
#include "INPUTS/INPUTS.h"
#include "PinInterface/PinInterface.h"
#include "BlinkController/BlinkController.h"
#include "IInputWithTimer/IInputWithTimer.h"
#include "ScoreBoard/ScoreBoard.h"
#include "PickleListenerContext/PickleListenerContext.h"
#include "RemotePairingScreen/RemotePairingScreen.h"
#include "PairingBlinker/PairingBlinker.h"
#include "ScoreboardBlinker/ScoreboardBlinker.h"
#include "BlankBlinker/BlankBlinker.h"
#include "MatchWinBlinker/MatchWinBlinker.h"
#include "IGameInput/IGameInput.h"
#include "RemoteLocker/RemoteLocker.h"
#include "KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "KeyboardGameInput/KeyboardGameInput.h"
#include "StateMachine/StateMachine.h"
#include "ClockTimer/ClockTimer.h"
#include "ClockUpdater/ClockUpdater.h"
#include "ConsoleDisplay/ConsoleDisplay.h"

void run_pickle_keyboard( int game_mode ) {
    std::cout << "Creating ClockTimer..." << std::endl;
    ClockTimer* clockTimer = new ClockTimer(); // Create the ClockTimer for tracking time in REGULAR_PLAY_AFTER_SCORE_STATE
    std::cout << "ClockTimer created successfully" << std::endl;
    
    std::cout << "About to create Rules object..." << std::endl;
    Rules* _rules = new Rules( 1 ); // represents the #players on each of the two teams
    std::cout << "Rules object created successfully" << std::endl;
    GameTimer* _gameTimer = new GameTimer();
    GameState* _gameState = new GameState();
    Team* _team_a = new Team( TEAM_A );
    Team* _team_b = new Team( TEAM_B );
    _team_a->setGameState( _gameState );
    _team_b->setGameState( _gameState );
    _team_a->setOpposingTeam( _team_b );
    _team_b->setOpposingTeam( _team_a );
    _team_a->setServe( 1 );
    PinState*_pinState = new PinState();
    PinInterface* _pinInterface = new PinInterface( _pinState );
    ColorManager* colorManager = new ColorManager();
    FontManager* fontManager = new FontManager();
    IDisplay* display = new ConsoleDisplay(colorManager);
    ScoreBoard* _scoreBoard = new ScoreBoard( _pinInterface, _team_a, _team_b, _rules, clockTimer, _gameState, display, fontManager, colorManager );
    
    
    std::cout << "Creating ClockUpdater..." << std::endl;
    ClockUpdater* clockUpdater = new ClockUpdater( clockTimer, _scoreBoard );

    std::cout << "ClockUpdater created successfully" << std::endl;Inputs* _gameInputs = new Inputs( _team_a, _team_b, _pinInterface, _scoreBoard );
    GameModes* _gameModes = new GameModes( _team_a, _team_b, _pinInterface, _rules, _scoreBoard );
    Reset* _reset = new Reset( _pinInterface );
    auto _history = std::make_shared<History>();
    BlinkController* _blinkController = new BlinkController( _pinInterface, _team_a, _team_b );
    _team_a->attach( _blinkController );
    _team_b->attach( _blinkController );
    GameObject* gameObject = new GameObject( _team_a, _team_b, _pinState, _pinInterface, _gameState, _gameTimer, _gameInputs, _gameModes, _scoreBoard );
    gameObject->setHistory( _history );
    gameObject->setRules(   _rules   );
    Team* team_a = gameObject->getTeamA();
    Team* team_b = gameObject->getTeamB();
    team_a->setServe( 1 );                     // start with green 1st 
    
    BlinkController* blinkController = new BlinkController( _pinInterface, team_a, team_b);

    IInputWithTimer* inputWithTimer         = nullptr;
    IInputWithTimer* noBlinkInputWithTimer  = nullptr;
    MatchWinBlinker* matchWinBlinker        = new MatchWinBlinker( _scoreBoard );
    PairingBlinker*  pairingBlinker         = new PairingBlinker(  _scoreBoard );
    BlankBlinker*       blankBlinker        = nullptr;
    ScoreboardBlinker*  scoreboardBlinker   = nullptr;

    RemoteLocker* remoteLocker;
    bool no_score = true;

    if ( game_mode == SINGLES_MODE ) {
        _rules->setFreshServes( 1  );
        _rules->setPointsToWin( 11 );
        _rules->setMaxPoints(   MAX_POINTS );
    } else {
        _rules->setFreshServes( 2  );
        _rules->setPointsToWin( 11 );
        _rules->setMaxPoints(   MAX_POINTS );
    }
    
    _gameModes->gameStart( _gameState, *_history );  // sets gameStart to true. resets player numbers and score board.
    _gameState->setGameRunning();  // otherwise, exits right away
    std::cout << "PickleBall Game Initialized with Keyboard Input. Starting main loop..." << std::endl;

    // Main game loop - configured specifically for keyboard input
    while ( _gameState->gameRunning() != 0 ) {
        std::cout << "Main loop iteration (keyboard mode)..." << std::endl;
        std::cout.flush();

        const int main_input_timeout = MAIN_INPUT_TIMEOUT;
        RemoteLocker* remoteLocker = new RemoteLocker( team_a );
        bool no_score = true;
        
        RemotePairingScreen* remotePairingScreen = new RemotePairingScreen( _scoreBoard, team_a, team_b );

        // set up blinkers
        std::shared_ptr<PairingBlinker> pairingBlinker = std::make_shared<PairingBlinker>( _scoreBoard );
        std::shared_ptr<ScoreboardBlinker> sleepingBlinker = std::make_shared<ScoreboardBlinker>( _scoreBoard );
        std::shared_ptr<BlankBlinker> blankBlinker = std::make_shared<BlankBlinker>();
        std::shared_ptr<MatchWinBlinker> matchWinBlinker = std::make_shared<MatchWinBlinker>( _scoreBoard );

        // declare the input handlers - specifically configured for keyboard
        IInputWithTimer* pairingInputWithTimer = nullptr;
        IInputWithTimer* noBlinkInputWithTimer = nullptr;
        IInputWithTimer* sleepingInputWithTimer = nullptr;
        IGameInput* gameInput = nullptr;

        // gameObject->loopGame(); // call loop game to initialize sets // took out 081125

        // create keyboard input handlers
        if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating KeyInputWithTimer! ***" );}
        pairingInputWithTimer = new KeyboardInputWithTimer( pairingBlinker.get(), main_input_timeout );
        noBlinkInputWithTimer = new KeyboardInputWithTimer( blankBlinker.get(), main_input_timeout );
        sleepingInputWithTimer = new KeyboardInputWithTimer( sleepingBlinker.get(), SLEEP_FOREVER );
        gameInput = new KeyboardGameInput();

        std::cout << "Keyboard input handlers initialized." << std::endl;

        PickleListenerContext context(  // create the state context
            _scoreBoard,
            gameObject,
            _gameState,
            _reset,
            _gameInputs,
            _pinInterface,
            blinkController,
            inputWithTimer,
            sleepingInputWithTimer,
            noBlinkInputWithTimer,
            pairingInputWithTimer,
            pairingBlinker.get(),
            blankBlinker.get(),
            matchWinBlinker.get(),
            remoteLocker,
            remotePairingScreen,
            clockTimer,
            clockUpdater,
            no_score
        );

        // initialize and run the StateMachine
        StateMachine stateMachine( context );
        stateMachine.run();

        // cleanup
        delete remoteLocker;
        delete pairingInputWithTimer;
        delete noBlinkInputWithTimer;
        delete sleepingInputWithTimer;
        delete gameInput;
        delete _gameInputs;
        delete remotePairingScreen;
        delete clockTimer;

        std::this_thread::sleep_for(std::chrono::milliseconds( 25 ));
    }

    std::cout << "Exiting PickleBall Game main loop." << std::endl;

    // Cleanup
    delete blinkController;
    delete _gameInputs;
    delete _reset;
    delete gameObject;
    delete _scoreBoard;
    delete _gameState;
}

int main(int argc, char* argv[]) {
    std::cout << "Initializing PickleBall Game System (Keyboard Mode)..." << std::endl;
    std::signal(SIGINT, [](int){ gSignalStatus = 1; }); // Simple signal handler

    int game_mode = DOUBLES_MODE;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--singles") {
            game_mode = SINGLES_MODE;
        } else if (arg == "--doubles") {
            game_mode = DOUBLES_MODE;
        } else {
            // std::cout << "Usage: " << argv[0] << " [--singles|--doubles]" << std::endl;
            // std::cout << "  --singles: Set game mode to singles (1 player per team)" << std::endl;
            // std::cout << "  --doubles: Set game mode to doubles (2 players per team)" << std::endl;
            // return 1;
            game_mode = SINGLES_MODE;
        }
    }

    run_pickle_keyboard( game_mode );

    std::cout << "Shutting down PickleBall Game System." << std::endl;
    return 0;
}