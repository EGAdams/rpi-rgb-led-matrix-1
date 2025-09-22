/************************************************************
 * run_pickle_listener.cpp
 *
 * Startup file for the PickleBall game using PickleListenerContext.
 * This file initializes the PickleBall game objects, input handlers,
 * and runs the main game loop.
 *
 * The structure is inspired by run_remote_listener.cpp for the tennis game,
 * but adapted to use PickleListenerContext and PickleBall-specific objects.
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
#include "RemoteInputWithTimer/RemoteInputWithTimer.h"
#include "TestGameInput/TestGameInput.h"
#include "TestInputWithTimer/TestInputWithTimer.h"
#include "KeyboardInputWithTimer/KeyboardInputWithTimer.h"
#include "RemoteGameInput/RemoteGameInput.h"
#include "KeyboardGameInput/KeyboardGameInput.h"
#include "StateMachine/StateMachine.h"
#include "ConsoleDisplay/ConsoleDisplay.h"
#include "ClockTimer/ClockTimer.h"
#include "ClockUpdater/ClockUpdater.h"

bool is_on_raspberry_pi() {
    std::ifstream file("/proc/device-tree/model");
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        file.close();
        if (line.find("Raspberry Pi") != std::string::npos) { return true; }
    }
    return false;
}

void run_pickle_listener() {
    ClockTimer* clockTimer = new ClockTimer();
    Rules* _rules = new Rules( 2 ); // represents the #players on each of the two teams (doubles)
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
    std::cout << "About to create ScoreBoard..." << std::endl;
    ScoreBoard* _scoreBoard = new ScoreBoard( _pinInterface, _team_a, _team_b, _rules, clockTimer, _gameState, display, fontManager, colorManager );
    std::cout << "ScoreBoard created successfully!" << std::endl;
    Inputs* _gameInputs = new Inputs( _team_a, _team_b, _pinInterface, _scoreBoard );
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

    // Create the PickleListenerContext


    _gameState->setGameRunning();  // otherwise, exits right away
    std::cout << "PickleBall Game Initialized. Starting main loop..." << std::endl;

    // Main game loop - simplified example
    while ( _gameState->gameRunning() != 0 ) {
        std::cout << "Main loop iteration..." << std::endl;
        std::cout.flush();

        // Here you would add game logic, input handling, state updates, etc.
        const int main_input_timeout = MAIN_INPUT_TIMEOUT; // <---------- set universal timeout here ---------<<
        // GameState* gameState = gameStatearg;
        RemoteLocker* remoteLocker = new RemoteLocker( team_a );
        bool no_score = true;
        // Inputs* inputs = new Inputs( gameObject->getTeamA(), gameObject->getTeamB(), gameObject->getPinInterface(), gameState );
        bool keyboard_off = true;     // <---------- toggle the keyboard off here ---------<<
        bool testing = true;
        // Use _scoreBoard directly for consistency
        // _scoreBoard->setLittleDrawerFont( "fonts/8x13B.bdf" );
        RemotePairingScreen* remotePairingScreen = new RemotePairingScreen( _scoreBoard, team_a, team_b );

        // set up blinkers
        std::shared_ptr<PairingBlinker> pairingBlinker = std::make_shared<PairingBlinker>( _scoreBoard );
        std::shared_ptr<ScoreboardBlinker> sleepingBlinker = std::make_shared<ScoreboardBlinker>( _scoreBoard );
        std::shared_ptr<BlankBlinker> blankBlinker = std::make_shared<BlankBlinker>();
        std::shared_ptr<MatchWinBlinker> matchWinBlinker = std::make_shared<MatchWinBlinker>( _scoreBoard );

        // declare the input handlers
        IInputWithTimer* pairingInputWithTimer = nullptr;
        IInputWithTimer* noBlinkInputWithTimer = nullptr;
        IInputWithTimer* sleepingInputWithTimer = nullptr;
        IGameInput* gameInput = nullptr;

        gameObject->loopGame(); // call loop game to initialize sets // 061225 may need to do this somewhere else

        // create the input handlers depending on the machine type
        if ( _scoreBoard->onRaspberryPi() && keyboard_off && !testing ) {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating RemoteInputWithTimer! ***" );}
            pairingInputWithTimer  = new RemoteInputWithTimer( pairingBlinker.get(), _gameInputs, main_input_timeout );
            noBlinkInputWithTimer  = new RemoteInputWithTimer( blankBlinker.get(), _gameInputs, main_input_timeout );
            sleepingInputWithTimer = new RemoteInputWithTimer( sleepingBlinker.get(), _gameInputs, SLEEP_FOREVER );
            gameInput = new RemoteGameInput( (IInputs*)_gameInputs );
        } else if ( testing ) {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating RemoteInputWithTimer! ***" );}
            pairingInputWithTimer  = new TestInputWithTimer( static_cast<Blinker*>(pairingBlinker.get()), _gameInputs, main_input_timeout );
            noBlinkInputWithTimer  = new TestInputWithTimer( static_cast<Blinker*>(blankBlinker.get()), _gameInputs, main_input_timeout );
            sleepingInputWithTimer = new TestInputWithTimer( static_cast<Blinker*>(sleepingBlinker.get()), _gameInputs, SLEEP_FOREVER );
            gameInput = new TestGameInput();
        } else {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating KeyInputWithTimer! ***" );}
            pairingInputWithTimer = new KeyboardInputWithTimer( pairingBlinker.get(), main_input_timeout );
            noBlinkInputWithTimer = new KeyboardInputWithTimer( blankBlinker.get(), main_input_timeout );
            sleepingInputWithTimer = new KeyboardInputWithTimer( sleepingBlinker.get(), SLEEP_FOREVER );
            gameInput = new KeyboardGameInput();
        }

        // since we have the ScoreBoard object instantiate now, 
        // create the ClockUpdater for tracking time in REGULAR_PLAY_AFTER_SCORE_STATE
        ClockUpdater* clockUpdater = new ClockUpdater( clockTimer, _scoreBoard );

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

int main() {
    std::cout << "Initializing PickleBall Game System..." << std::endl;
    std::signal(SIGINT, [](int){ gSignalStatus = 1; }); // Simple signal handler

    run_pickle_listener();

    std::cout << "Shutting down PickleBall Game System." << std::endl;
    return 0;
}