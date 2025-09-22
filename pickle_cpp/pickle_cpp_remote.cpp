/************************************************************
 * pickle_cpp_remote.cpp
 *
 * Startup file for the PickleBall game using PickleListenerContext.
 * This file initializes the PickleBall game objects, input handlers,
 * and runs the main game loop.
 *
 * The structure is inspired by run_remote_listener.cpp for the tennis game,
 * but adapted to use PickleListenerContext and PickleBall-specific objects.
 * This version uses RemoteInputWithTimer instead of TestInputWithTimer.
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
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open() ) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

void run_pickle_remote( int game_mode ) {
    GameState* _gameState = new GameState();
    _gameState->setGameMode( game_mode );
    Rules* _rules = new Rules( _gameState->getGameMode() ); // represents the #players on each of the two teams
    GameTimer* _gameTimer = new GameTimer();
    Team* _team_a = new Team( TEAM_A );
    Team* _team_b = new Team( TEAM_B );
    _team_a->setGameState( _gameState );
    _team_b->setGameState( _gameState );
    _team_a->setOpposingTeam( _team_b );
    _team_b->setOpposingTeam( _team_a );
    _team_a->setServe( 1 );
    PinState* _pinState = new PinState();
    PinInterface* _pinInterface = new PinInterface( _pinState );
    ColorManager* colorManager = new ColorManager();
    FontManager* fontManager = new FontManager();
    IDisplay* display = new ConsoleDisplay( colorManager );
    std::cout << "About to create ScoreBoard..." << std::endl;
    // ScoreBoard* _scoreBoard = new ScoreBoard(_team_a, _team_b, _gameState, display, fontManager, colorManager, _rules);
    ClockTimer* clockTimer = new ClockTimer();
    ScoreBoard* _scoreBoard = new ScoreBoard( _pinInterface, _team_a, _team_b, _rules, clockTimer, _gameState, display, fontManager, colorManager );

    std::cout << "Creating ClockUpdater..." << std::endl;
    ClockUpdater* clockUpdater = new ClockUpdater( clockTimer, _scoreBoard );
    std::cout << "ClockUpdater created successfully" << std::endl;

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
    gameObject->setRules( _rules );
    Team* team_a = gameObject->getTeamA();
    Team* team_b = gameObject->getTeamB();
    team_a->setServe( 1 );                     // go with green 1st

    BlinkController* blinkController = new BlinkController( _pinInterface, team_a, team_b );

    IInputWithTimer* inputWithTimer = nullptr;
    IInputWithTimer* noBlinkInputWithTimer = nullptr;
    MatchWinBlinker* matchWinBlinker = new MatchWinBlinker( _scoreBoard );
    PairingBlinker* pairingBlinker = new PairingBlinker( _scoreBoard );
    BlankBlinker* blankBlinker = nullptr;
    ScoreboardBlinker* scoreboardBlinker = nullptr;

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
        bool testing = false;        // <---------- Use RemoteInputWithTimer instead of TestInputWithTimer ---------<<
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

        _gameModes->gameStart( _gameState, *_history );  // sets gameStart to true. resets player numbers and score board.
        // gameObject->loopGame(); // call loop game to initialize sets // 061225 may need to do this somewhere else

        // create the input handlers depending on the machine type
        if ( _scoreBoard->onRaspberryPi() && keyboard_off && !testing ) {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating RemoteInputWithTimer! ***" ); }
            pairingInputWithTimer = new RemoteInputWithTimer( pairingBlinker.get(), _gameInputs, main_input_timeout );
            noBlinkInputWithTimer = new RemoteInputWithTimer( blankBlinker.get(), _gameInputs, main_input_timeout );
            sleepingInputWithTimer = new RemoteInputWithTimer( sleepingBlinker.get(), _gameInputs, SLEEP_FOREVER );
            gameInput = new RemoteGameInput( ( IInputs* ) _gameInputs );
        }
        else if ( testing ) {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating TestInputWithTimer! ***" ); }
            pairingInputWithTimer = new TestInputWithTimer( static_cast< Blinker* >( pairingBlinker.get() ), _gameInputs, main_input_timeout );
            noBlinkInputWithTimer = new TestInputWithTimer( static_cast< Blinker* >( blankBlinker.get() ), _gameInputs, main_input_timeout );
            sleepingInputWithTimer = new TestInputWithTimer( static_cast< Blinker* >( sleepingBlinker.get() ), _gameInputs, SLEEP_FOREVER );
            gameInput = new TestGameInput();
        }
        else {
            if ( !pairingBlinker ) { print( "*** ERROR: pairingBlinker is NULL before creating KeyboardInputWithTimer! ***" ); }
            pairingInputWithTimer = new KeyboardInputWithTimer( pairingBlinker.get(), main_input_timeout );
            noBlinkInputWithTimer = new KeyboardInputWithTimer( blankBlinker.get(), main_input_timeout );
            sleepingInputWithTimer = new KeyboardInputWithTimer( sleepingBlinker.get(), SLEEP_FOREVER );
            gameInput = new KeyboardGameInput();
        }

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

        std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );
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

int main( int argc, char* argv[] ) {  // Parse command line arguments for game mode
    std::cout << "Initializing PickleBall Game System (Remote)..." << std::endl;
    std::signal( SIGINT, []( int ) { gSignalStatus = 1; } ); // Simple signal handler

    int gameMode = DOUBLES_MODE; // Default to doubles mode

    for ( int i = 1; i < argc; i++ ) {
        std::string arg = argv[i];
        if ( arg == "--singles" ) {
            gameMode = SINGLES_MODE;
        }
        else if ( arg == "--doubles" ) {
            gameMode = DOUBLES_MODE;
        }
        else {
            std::cout << "Usage: " << argv[0] << " [--singles|--doubles]" << std::endl;
            std::cout << "  --singles: Set game mode to singles (1 player per team)" << std::endl;
            std::cout << "  --doubles: Set game mode to doubles (2 players per team)" << std::endl;
            return 1;
        }
    }

    run_pickle_remote( gameMode );

    std::cout << "Shutting down PickleBall Game System (Remote)." << std::endl;
    return 0;
}
