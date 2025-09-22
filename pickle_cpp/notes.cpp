/*
 * c++ notes
 */

PinInterface* goPinInt = _gameObject->getPinInterface();
// std::cout << (reinterpret_cast<const char*>(std::addressof(_pinInterface)) ==
reinterpret_cast< const char* >( std::addressof( goPinInt ) ))
<< std::endl;

Napi::Value GetValue( const Napi::CallbackInfo& info );
Napi::Value PlusOne( const Napi::CallbackInfo& info );
Napi::Value Multiply( const Napi::CallbackInfo& info );
Napi::Value digitalWrite( const Napi::CallbackInfo& info );
Napi::Value digitalRead( const Napi::CallbackInfo& info );
Napi::Value analogRead( const Napi::CallbackInfo& info );
Napi::Value gameLoop( const Napi::CallbackInfo& info );
Napi::Value getPinMap( const Napi::CallbackInfo& info );

// int anlgPlyrBtnVal = _pinInterface->pinAnalogRead( PLAYER_BUTTONS );
// if ( anlgPlyrBtnVal <= 1000 ) {  // if one of the player buttons is pressed...

//     GameTimer::gameDelay( 20 );
//     if (      anlgPlyrBtnVal <= 50 ) {                           gameState->setPlayerButton( 1 ); }
//     else if ( anlgPlyrBtnVal >= 350 && anlgPlyrBtnVal <= 400 ) { gameState->setPlayerButton( 2 ); }
//     else if ( anlgPlyrBtnVal >= 550 && anlgPlyrBtnVal <= 650 ) { gameState->setPlayerButton( 3 ); }
//     else if ( anlgPlyrBtnVal >= 750 && anlgPlyrBtnVal <= 800 ) { gameState->setPlayerButton( 4 ); }
//     while ( _pinInterface->pinAnalogRead( PLAYER_BUTTONS ) <= 1000 ) {
//         #if defined _WIN32
//         GameTimer::gameDelay( 20 );
//         _logger->logUpdate( "Player button " + std::to_string( gameState->getPlayerButton()) + " pressed", "readPlayerButtons" );
//         #endif
//     }
//     #if defined _WIN32
//     _logger->logUpdate( "Player button " + std::to_string( gameState->getPlayerButton()) + " released", "readPlayerButtons" );
//     #endif
// }

