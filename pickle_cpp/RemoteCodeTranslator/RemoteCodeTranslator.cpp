
/** @class  RemoteCodeTranslator */
#include "RemoteCodeTranslator.h"
#include "../Arduino/Arduino.h"

RemoteCodeTranslator::RemoteCodeTranslator(){
    #if defined _WIN32
    _logger     = new Logger( "RemoteCodeTranslator" );
    _translator = new TranslateConstant();
    #endif 
}
RemoteCodeTranslator::~RemoteCodeTranslator(){}

int RemoteCodeTranslator::translateRemoteCode( remoteDataStructure remote ) { 
    #if defined _WIN32
    // _logger->logUpdate( " inputs [" + std::to_string( remote.pin_1 ) + "], " +
    //                             "[" + std::to_string( remote.pin_2 ) + "], " +
    //                             "[" + std::to_string( remote.pin_3 ) + "], " +
    //                             "[" + std::to_string( remote.pin_4 ) + "]  ", "translatedRemoteCode()" );
    #endif  
    // RED REMOTE
    if ( remote.pin_1 == 1 && remote.pin_2 == 1 && remote.pin_3 == 1 && remote.pin_4 == 0 ) { return RED_REMOTE_RED_SCORE;     }
    if ( remote.pin_1 == 1 && remote.pin_2 == 1 && remote.pin_3 == 0 && remote.pin_4 == 1 ) { return RED_REMOTE_GREEN_SCORE;   }
    if ( remote.pin_1 == 1 && remote.pin_2 == 1 && remote.pin_3 == 0 && remote.pin_4 == 0 ) { return RED_REMOTE_UNDO;          }
    if ( remote.pin_1 == 1 && remote.pin_2 == 0 && remote.pin_3 == 1 && remote.pin_4 == 1 ) { return RED_REMOTE_RESET;         }

    // GREEN REMOTE
    if ( remote.pin_1 == 0 && remote.pin_2 == 1 && remote.pin_3 == 1 && remote.pin_4 == 0 ) { return GREEN_REMOTE_GREEN_SCORE; }
    if ( remote.pin_1 == 0 && remote.pin_2 == 1 && remote.pin_3 == 0 && remote.pin_4 == 1 ) { return GREEN_REMOTE_RED_SCORE;   }
    if ( remote.pin_1 == 0 && remote.pin_2 == 1 && remote.pin_3 == 0 && remote.pin_4 == 0 ) { return GREEN_REMOTE_UNDO;        }
    if ( remote.pin_1 == 0 && remote.pin_2 == 0 && remote.pin_3 == 1 && remote.pin_4 == 1 ) { return GREEN_REMOTE_RESET;       }
    if ( remote.pin_1 == 1 && remote.pin_2 == 1 && remote.pin_3 == 1 && remote.pin_4 == 1 ) { return UNKNOWN_REMOTE_BUTTON;    }
    // _logger->logUpdate( "returning UNKNOWN_REMOTE_BUTTON... ", "translateRemoteCode()" );
    return UNKNOWN_REMOTE_BUTTON;
}
