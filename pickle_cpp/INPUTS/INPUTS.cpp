#include "INPUTS.h"
#include "../Arduino/Arduino.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../Logger/Logger.h"

Inputs::Inputs( Team* team_a, Team* team_b, PinInterface* pinInterface, ScoreBoard* scoreBoard ) :
    _team_a( team_a ),
    _team_b( team_b ),
    _pinInterface( pinInterface ),
    _scoreboard( scoreBoard ) { 
        _gameStateManager = new GameStateManager( scoreBoard );
        _remoteCodeTranslator = new RemoteCodeTranslator();
        #if !defined( ARDUINO_TEENSY41 )
        _logger = new Logger( "Inputs" );
        #endif 
    }

Inputs::~Inputs() {};

int Inputs::read_mcp23017_value() {
     int originalRemoteCode = _pinInterface->read_mcp23017_value();
     if ( originalRemoteCode == UNKNOWN_REMOTE_BUTTON ) { return originalRemoteCode; }
     if ( _scoreboard == nullptr ) {
         print( "scoreboard is null.  not flashing. ");
     }
    print( "\n*** possibly valid RemoteCode [" + std::to_string( originalRemoteCode ) + "] ***\n" );
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    print( "getting another remote code after STEVE_DELAY to verify" );
    int freshRemoteCode = _pinInterface->read_mcp23017_value();
    print( "*** freshRemoteCode after steve delay [" + std::to_string( freshRemoteCode ) + "] ***\n\n" );
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        print(  "*** got a match.  entering while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) ***\n" );
        
        // Add timeout protection to prevent infinite loop
        unsigned long button_release_start = GameTimer::gameMillis();
        const unsigned long BUTTON_RELEASE_TIMEOUT = 2000; // 2 seconds max to wait for button release
        
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            // print( "waiting " << REMOTE_READ_DELAY << "ms to detect button release..." );
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait remote read delay, then get a fresh one...
            freshRemoteCode = _pinInterface->read_mcp23017_value();
            
            // Check for timeout to prevent infinite loop
            unsigned long elapsed = GameTimer::gameMillis() - button_release_start;
            if ( elapsed > BUTTON_RELEASE_TIMEOUT ) {
                print( "*** WARNING: Button release timeout after " << elapsed << "ms. Breaking out of loop. ***" );
                break;
            }
        }
        print("\n\nexited while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ))" );
        std::cout << "\n\n\n***************************************************************" << std::endl;
        print("\n*** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]\n\n" );
        std::cout << "***************************************************************\n\n\n" << std::endl;
        return originalRemoteCode;
    } else {                       // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON;
    }
 }

void Inputs::readReset( Reset* reset, GameState* gameState, History& history ) {
    #if !defined( ARDUINO_TEENSY41 )
    if ( _pinInterface->pinAnalogRead( RESET ) == LOW ) {
        while ( _pinInterface->pinAnalogRead( RESET ) == LOW ) { GameTimer::gameDelay( 25 ); }
        reset->resetScoreboard( gameState );
        _gameStateManager->saveGameState( gameState, _team_a, _team_b, history ); }
    #else
    if ( _pinInterface->pinDigitalRead( RESET ) == LOW ) {
        while ( _pinInterface->pinDigitalRead( RESET ) == LOW ) { GameTimer::gameDelay( 25 ); }
        GameTimer::gameDelay( 100 );
        reset->resetScoreboard( gameState );
        _gameStateManager->saveGameState( gameState, _team_a, _team_b, history ); }
    #endif
}

void Inputs::readUndoButton( GameState* gameState ) {
    #if !defined( ARDUINO_TEENSY41 )
    if ( _pinInterface->pinAnalogRead( UNDO ) == LOW ) {
        while ( _pinInterface->pinAnalogRead( UNDO ) == LOW ) { GameTimer::gameDelay( 25 ); }
        // gameState->setUndoFlag( 1 );
        }
    #else    
    if ( _pinInterface->pinDigitalRead( UNDO ) == LOW ) {
        while ( _pinInterface->pinDigitalRead( UNDO ) == LOW ) { GameTimer::gameDelay( 25 ); }
        GameTimer::gameDelay( 100 );
        // gameState->setUndoFlag( 1 ); 
    }
    #endif
    }

int Inputs::readRotary( GameState* gameState, Reset* reset ) {
    gameState->setRotaryPosition( 0 );  
    int rotaryAnalogValue = _pinInterface->pinAnalogRead( ROTARY ); // Fix: Define the identifier "A15"
    if ( rotaryAnalogValue <= 100 ) { gameState->setRotaryPosition( 1 ); } 
    if ( rotaryAnalogValue >= 350 && rotaryAnalogValue <= 450 ) {  gameState->setRotaryPosition( 2 ); }
    if ( rotaryAnalogValue >= 550 && rotaryAnalogValue <= 700 ) {  gameState->setRotaryPosition( 3 ); }
    if ( rotaryAnalogValue >= 750 && rotaryAnalogValue <= 800 ) {  gameState->setRotaryPosition( 4 ); }
    if ( rotaryAnalogValue >= 850 && rotaryAnalogValue <= 1000 ) { gameState->setRotaryPosition( 5 ); }

    if ( gameState->getRotaryPosition() != gameState->getPrevRotaryPosition() ) {
        gameState->setRotaryChange( 1 );  // rotaryChange = true;
        gameState->setPrevRotaryPosition( gameState->getRotaryPosition() );  //  prevRotaryPosition = rotaryPosition;
        reset->refresh( gameState ); } // set rotaryChange to false and reset the scoreboard
    
    return gameState->getRotaryPosition(); }

int Inputs::readRemotePinArray( remoteDataStructure* remoteData ) { 
    #if !defined( ARDUINO_TEENSY41 )
    remoteData->pin_1 = _pinInterface->pinAnalogRead( REMOTE_DATA_0 );
    remoteData->pin_2 = _pinInterface->pinAnalogRead( REMOTE_DATA_1 );
    remoteData->pin_3 = _pinInterface->pinAnalogRead( REMOTE_DATA_2 );
    remoteData->pin_4 = _pinInterface->pinAnalogRead( REMOTE_DATA_3 );
    #else 
    remoteData->pin_1 = _pinInterface->pinDigitalRead( REMOTE_DATA_0 );
    remoteData->pin_2 = _pinInterface->pinDigitalRead( REMOTE_DATA_1 );
    remoteData->pin_3 = _pinInterface->pinDigitalRead( REMOTE_DATA_2 );
    remoteData->pin_4 = _pinInterface->pinDigitalRead( REMOTE_DATA_3 );
    #endif
    return _remoteCodeTranslator->translateRemoteCode( *remoteData ); }

int Inputs::readPlayerButtons() {
    remoteDataStructure remoteInitialData = { 0, 0, 0, 0 };
    remoteDataStructure *remoteData = &remoteInitialData;
    int originalRemoteCode = this->readRemotePinArray( remoteData );
    #if !defined( ARDUINO_TEENSY41 )
    // std::cout << "got original remote code: " << originalRemoteCode << std::endl;
    #endif
    GameTimer::gameDelay( STEVE_DELAY ); // that delay steve was talking about...
    int freshRemoteCode = this->readRemotePinArray( remoteData );
    #if !defined( ARDUINO_TEENSY41 )
    if ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON ) {
        std::cout << "got fresh remote code: " << std::to_string( freshRemoteCode ) << std::endl;
    }
    #endif
    if (( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {  // known code and a match?
        while(( freshRemoteCode == originalRemoteCode ) && ( freshRemoteCode != UNKNOWN_REMOTE_BUTTON )) {
            #if !defined( ARDUINO_TEENSY41 )  // got a matching measurement gameDelay( x ) apart.  entering while...
            std::cout <<  "inside while.  freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
            #endif
            GameTimer::gameDelay( REMOTE_READ_DELAY ); //  wait 250ms, then get a fresh one...
            #if !defined( ARDUINO_TEENSY41 )
            std::cout << "after delay within while reading getting fresh remote code again to verify" << std::endl;
            #endif
            freshRemoteCode = this->readRemotePinArray( remoteData );
            #if !defined( ARDUINO_TEENSY41 )
            _logger->logUpdate( " inputs [" + std::to_string( remoteData->pin_1 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_2 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_3 ) + "], " +
                                        "[" + std::to_string( remoteData->pin_4 ) + "]  ", "translatedRemoteCode()" ); 
            std::cout << "after delay within while; freshRemoteCode [" << std::to_string( freshRemoteCode ) << "]" << std::endl;
            #endif
        }
        #if !defined( ARDUINO_TEENSY41 )
        std::cout << "\n\n\n***************************************************************" << std::endl;
        std::cout << "exited while.  *** CODE IS VALID ***.  returning originalRemoteCode [" << std::to_string( originalRemoteCode ) << "]" << std::endl;
        std::cout << "***************************************************************\n\n\n" << std::endl;
        #endif
        return originalRemoteCode;
    } else { // false alarm.  we DO NOT have a matching measurement gameDelay( x ) apart.
        return UNKNOWN_REMOTE_BUTTON; }
    }
